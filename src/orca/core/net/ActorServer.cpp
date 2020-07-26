#include "ActorServer.h"
#include "TcpPacket.h"
#include "Protocol.h"

#include "../../base/error/ErrorHandle.h"

using namespace orca::core;
using namespace std;

const int ActorServer::HeartTimeOutSecend = 120;

ActorServer::ActorServer(uv::EventLoop* loop, uint32_t id, OnActorMeessageCallback callback)
    :uv::TcpServer(loop),
    id_(id),
    onActorMessageCallback_(callback)
{
    setTimeout(HeartTimeOutSecend);
    setMessageCallback(std::bind(&ActorServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}


void orca::core::ActorServer::onMessage(std::shared_ptr<uv::TcpConnection>connection, const char* data, ssize_t size)
{
    auto packetBuf = connection->getPacketBuffer();
    if (nullptr != packetBuf)
    {
        packetBuf->append(data, (int)size);
        TcpPacket packet;
        while (0 == packetBuf->readGeneric(&packet))
        {
            switch (packet.messageType_)
            {
            case Protocol::ReqFrameworkId:
            {
                uint32_t id = *((uint32_t*)packet.getData());
                onReqFrameworkId(id, connection);
                break;
            }
            case Protocol::ActorMessage:
                onActorMessage(packet.getData(), packet.DataSize());
                break;
            default:
                break;
            }
        }
    }
}

void orca::core::ActorServer::onReqFrameworkId(uint32_t id, std::shared_ptr<uv::TcpConnection> connection)
{
    
    //resp remote framework id.
    TcpPacket packet;
    packet.messageType_ = Protocol::RespFrameworkId;
    packet.packWithType((const char*)(&id_), sizeof(uint32_t));
    connection->write(packet.Buffer().c_str(), packet.PacketSize(), 
        [this](uv::WriteInfo info)
    {
        if (0 != info.status)
        {
            base::ErrorHandle::Instance()->error(base::ErrorInfo::UVWriteFail,uv::EventLoop::GetErrorMessage(info.status));
        }
    });
    endPoints_[id] = connection;
}

void orca::core::ActorServer::onActorMessage(const char* data, int size)
{
    if (onActorMessageCallback_)
        onActorMessageCallback_(data, size);
}
