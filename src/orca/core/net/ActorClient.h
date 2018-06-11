#ifndef    ORCA_ACTOR_CLIENT_H
#define    ORCA_ACTOR_CLIENT_H

#include <memory>
#include  "../../base/libuv_cpp11/uv/uv11.h"

namespace orca
{

namespace core
{
class ActorClient : public uv::TcpClient
{
public:
    ActorClient(uv::EventLoop* loop,uv::SocketAddr& addr);
    void connect();
    void onConnect(bool isSuccessed);
    void onDisconnect();
private:
    uv::SocketAddr addr_;
    
    static const int ReconectTimeMS;
private:
    void reconnect();
};
using ActorClientPtr = std::shared_ptr<ActorClient>;
}
}
#endif // !   ORCA_CORE_ACTOR_CLIENT_H

