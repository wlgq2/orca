#include "EndPoint.h"

using namespace orca::core;

orca::core::EndPoint::EndPoint(EndPointAddress& addr, uint32_t id)
    :id_(id)
{
    uv::SocketAddr uvaddr(addr.ip, addr.port, static_cast<uv::SocketAddr::IPV>(addr.ipv));
    server_ = std::make_shared<ActorServer>(&loop_, uvaddr,id_);
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

void orca::core::EndPoint::appendRemoteEndPoint(EndPointAddress& addr)
{
    uv::SocketAddr socketAddr(addr.ip,addr.port,static_cast<uv::SocketAddr::IPV>(addr.ipv));
    ActorClientPtr client = std::make_shared<ActorClient>(&loop_, socketAddr);
    endPoints_.push_back(client);
}


void orca::core::EndPoint::clear()
{
    endPoints_.clear();
}
