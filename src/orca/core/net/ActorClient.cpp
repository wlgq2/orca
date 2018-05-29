#include "ActorClient.h"
#include "../ErrorHandle.h"

using namespace orca::core;
using namespace std;

const int ActorClient::ReconectTimeMS = 1500;

ActorClient::ActorClient(uv::EventLoop* loop,uv::SocketAddr& addr)
    :uv::TcpClient(loop),
    addr_(addr)
{
    setConnectCallback(std::bind(&ActorClient::onConnect,this,std::placeholders::_1));
    setConnectCloseCallback(std::bind(&ActorClient::onDisconnect, this));
}

void orca::core::ActorClient::connect()
{
    uv::TcpClient::connect(addr_);
}

void orca::core::ActorClient::onConnect(bool isSuccessed)
{
    if (!isSuccessed)
    {
        ErrorHandle::Instance()->error(ErrorHandle::UVConnectFail,std::string("connect server fail:")+addr_.toStr());
        reconnect();
    }
}

void orca::core::ActorClient::onDisconnect()
{
    ErrorHandle::Instance()->error(ErrorHandle::UVDisconnectFromServer, std::string("disconnect from server:") + addr_.toStr());
    reconnect();
}

void orca::core::ActorClient::reconnect()
{
    uv::Timer<void*>* timer = new uv::Timer<void*>(loop_, ReconectTimeMS, 0,
        [this](uv::Timer<void*>* timer, void*)
    {
        this->connect();
        timer->close([timer]() {delete timer; });
    },
       nullptr );
}
