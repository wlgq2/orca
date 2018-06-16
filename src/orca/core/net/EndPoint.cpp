#include "EndPoint.h"
#include "../../base/error/ErrorHandle.h"

using namespace orca::core;

orca::core::EndPoint::EndPoint(EndPointAddress& addr, uint32_t id)
    :id_(id),
    remoteRegisterCompleted_(false)
{
    uv::SocketAddr uvaddr(addr.ip, addr.port, static_cast<uv::SocketAddr::IPV>(addr.ipv));
    server_ = std::make_shared<ActorServer>(&loop_, uvaddr,id_);
}

void orca::core::EndPoint::run()
{
    remoteRegisterCompleted_ = endPoints_.empty() ? true: false;
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
    ActorClientPtr client = std::make_shared<ActorClient>(&loop_, socketAddr,id_);
    client->setRegisterRemoteFrameworkCallback(std::bind(&EndPoint::registerActorClient,this,std::placeholders::_1,std::placeholders::_2));
    endPoints_.push_back(client);
    
}


void orca::core::EndPoint::clear()
{
    endPoints_.clear();
}

void orca::core::EndPoint::registerActorClient(uint32_t id, ActorClientPtr client)
{
    //thread Safety
    loop_.runInThisLoop([this, client,id]()
    {
        if (id == id_)
        {
            base::ErrorHandle::Instance()->error(base::ErrorInfo::RepeatedRemoteFrameworkID,std::string("remote framework id repeated :")+std::to_string(id));
        }
        else
        {
            endPointMap_[id] = client;
            if (endPointMap_.size() == endPoints_.size())
            {
                remoteRegisterCompleted_ = true;
            }
        }
    });
}
