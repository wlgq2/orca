#ifndef    ORCA_ACTOR_SERVER_H
#define    ORCA_ACTOR_SERVER_H

#include  "../../base/libuv_cpp11/uv/uv11.h"

namespace orca
{

namespace core
{
using OnActorMeessageCallback = std::function<void()>;
class ActorServer : public uv::TcpServer
{
public:
    ActorServer(uv::EventLoop* loop,uv::SocketAddr& addr,uint32_t id);
    void onMessage(std::shared_ptr<uv::TcpConnection>connection, const char* data, ssize_t size);
    static const int HeartTimeOutSecend;
private:
    uint32_t id_;
};
}
}
#endif // !   ORCA_ACTOR_SERVER_H

