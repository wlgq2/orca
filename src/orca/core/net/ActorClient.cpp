#include "ActorClient.h"
#include <functional>

#include "../../base/error/ErrorHandle.h"
#include "Protocol.h"

using namespace orca::core;
using namespace std;

const int ActorClient::ReconectTimeMS = 1500;
const int ActorClient::HeartbeatTimeSec = 30;

ActorClient::ActorClient(uv::EventLoop* loop,uv::SocketAddr& addr,uint32_t id)
    :uv::TcpClient(loop),
    addr_(addr),
    isConenected_(false),
    localId_(id),
    remoteId_(-1),
    onRegisterRemoteFramework_(nullptr)
{
    timer_ = new uv::Timer(loop, 1000, 1000, std::bind(&ActorClient::heartbeat, this, std::placeholders::_1));
    setConnectStatusCallback(std::bind(&ActorClient::onConnectStatus,this,std::placeholders::_1));
    setMessageCallback(std::bind(&ActorClient::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    timer_->start();
}

orca::core::ActorClient::~ActorClient()
{
    timer_->close([](uv::Timer* timer)
    {
        //release timer in loop callback.
        delete timer;
    });
}

void orca::core::ActorClient::setRegisterRemoteFrameworkCallback(OnRegisterRemoteFramework callback)
{
    onRegisterRemoteFramework_ = callback;
}

void orca::core::ActorClient::connect()
{
    uv::TcpClient::connect(addr_);
}

void orca::core::ActorClient::onConnectStatus(uv::TcpClient::ConnectStatus status)
{
    switch (status)
    {
    case uv::TcpClient::OnConnectSuccess:
        isConenected_ = true;
        break;
    case uv::TcpClient::OnConnnectFail :
        isConenected_ = false;
        base::ErrorHandle::Instance()->error(base::ErrorInfo::UVConnectFail, std::string("connect server fail:") + addr_.toStr());
        reconnect();
        break;
    case uv::TcpClient::OnConnnectClose:
        isConenected_ = false;
        base::ErrorHandle::Instance()->error(base::ErrorInfo::UVDisconnectFromServer, std::string("disconnect from server:") + addr_.toStr());
        reconnect();
        break;
    default:
        break;
    }
}

void orca::core::ActorClient::onMessage(const char* data, ssize_t size)
{
    appendToBuffer(data, (int)size);
    uv::Packet packet;
    while (0 == readFromBuffer(packet))
    {
        if (Protocol::RespFrameworkId == packet.reserve_)
        {
            uint32_t id = *((uint32_t*)packet.getData());
            remoteId_ = id;
            if (onRegisterRemoteFramework_)
                onRegisterRemoteFramework_(remoteId_,shared_from_this());
        }
    }
}

void orca::core::ActorClient::reconnect()
{
    uv::Timer* timer = new uv::Timer(loop_, ReconectTimeMS, 0,
        [this](uv::Timer* timer)
    {
        this->connect();
        timer->close([](uv::Timer* timer)
        {
            //release timer in loop callback.
            delete timer; 
        });
    });
    timer->start();
}

void orca::core::ActorClient::heartbeat(uv::Timer*)
{
    if (isConenected_)
    {
        if (++cnt_ > HeartbeatTimeSec)
        {
            cnt_ = 0;
            //send heartbeat.
            uv::Packet packet;
            char null = 0x00;
            packet.reserve_ = Protocol::HeartBeatMessage;
            packet.fill(&null, sizeof(char));
            write(packet.Buffer(), packet.BufferSize());
        }
        if (remoteId_ < 0)
        {
            //req remote framework id.
            uv::Packet packet;
            uint32_t id = localId_;
            packet.reserve_ = Protocol::ReqFrameworkId;
            packet.fill((const char*)(&id), sizeof(uint32_t));
            write(packet.Buffer(), packet.BufferSize(),
                [this](uv::WriteInfo info)
            {
                if (0 != info.status)
                {
                    base::ErrorHandle::Instance()->error(base::ErrorInfo::UVWriteFail, uv::EventLoop::GetErrorMessage(info.status));
                }
            });
        }
    }
}
