#include "EndPoint.h"

using namespace orca::core;

orca::core::EndPoint::EndPoint(std::string ip, uint16_t port, EndPointAddr::IPV ipv)
{
    uv::SocketAddr addr(ip, port, static_cast<uv::SocketAddr::IPV>(ipv));
    server_ = std::make_shared<ActorServer>(&loop_, addr);
}

void orca::core::EndPoint::run()
{
    for (auto it = endPoints_.begin(); it != endPoints_.end(); it++)
    {
        (*it)->connect();
    }
    server_->start();
    loop_.run();
}

void orca::core::EndPoint::appendRemoteEndPoint(EndPointAddr& addr)
{
    uv::SocketAddr socketAddr(addr.ip,addr.port,static_cast<uv::SocketAddr::IPV>(addr.ipv));
    ActorClientPtr client = std::make_shared<ActorClient>(&loop_, socketAddr);
    endPoints_.push_back(client);
}

void orca::core::EndPoint::appendRemoteEndPoint(std::string ip, uint16_t port, EndPointAddr::IPV ipv)
{
    EndPointAddr addr = { ip,port,ipv };
    appendRemoteEndPoint(addr);
}

void orca::core::EndPoint::clear()
{
    endPoints_.clear();
}
