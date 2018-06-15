#include "ActorServer.h"

using namespace orca::core;
using namespace std;

const int ActorServer::HeartTimeOutSecend = 120;

ActorServer::ActorServer(uv::EventLoop* loop, uv::SocketAddr& addr, uint32_t id)
    :uv::TcpServer(loop,addr),
    id_(id)
{
    setTimeout(HeartTimeOutSecend);
    setMessageCallback(std::bind(&ActorServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}


void orca::core::ActorServer::onMessage(std::shared_ptr<uv::TcpConnection>connection , const char* data, ssize_t size)
{

}
