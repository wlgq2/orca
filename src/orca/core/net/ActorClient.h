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
    ~ActorClient();

    void connect();
    void onConnect(bool isSuccessed);
    void onDisconnect();
    void onMessage(const char* data, ssize_t size);

    static const int ReconectTimeMS;
    static const int HeartbeatTimeSec;
private:
    uv::SocketAddr addr_;
    bool isConenected_;
    uv::Timer<void*>* timer_;
    int frameworkId_;
    int cnt_;

private:
    void reconnect();
    void heartbeat(uv::Timer<void*>*,void*);

};
using ActorClientPtr = std::shared_ptr<ActorClient>;
}
}
#endif // !   ORCA_CORE_ACTOR_CLIENT_H

