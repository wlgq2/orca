#include "ActorServer.h"
#include "Protocol.h"

#include "../../base/error/ErrorHandle.h"

using namespace orca::core;
using namespace std;

const int ActorServer::HeartTimeOutSecend = 120;

ActorServer::ActorServer(uv::EventLoop* loop, uv::SocketAddr& addr, uint32_t id, OnActorMeessageCallback callback)
    :uv::TcpServer(loop,addr),
    id_(id),
    onActorMessageCallback_(callback)
{
    setTimeout(HeartTimeOutSecend);
    setMessageCallback(std::bind(&ActorServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}


void orca::core::ActorServer::onMessage(std::shared_ptr<uv::TcpConnection>connection, const char* data, ssize_t size)
{
    connection->appendToBuffer(data, (int)size);
    uv::Packet packet;
    while (0 == connection->readFromBuffer(packet))
    {
        switch (packet.reserve_)
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

void orca::core::ActorServer::onReqFrameworkId(uint32_t id, std::shared_ptr<uv::TcpConnection> connection)
{
    
    //resp remote framework id.
    uv::Packet packet;
    packet.reserve_ = Protocol::RespFrameworkId;
    packet.fill((const char*)(&id_), sizeof(uint32_t));
    connection->write(packet.Buffer(), packet.BufferSize(), 
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
