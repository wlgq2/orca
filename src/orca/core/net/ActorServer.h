#ifndef    ORCA_ACTOR_SERVER_H
#define    ORCA_ACTOR_SERVER_H

#include <map>
#include <memory>

#include  "../../base/libuv_cpp11/uv/uv11.h"

namespace orca
{

namespace core
{
using OnActorMeessageCallback = std::function<void(const char*, int)>;
class ActorServer : public uv::TcpServer
{
public:
    ActorServer(uv::EventLoop* loop,uv::SocketAddr& addr,uint32_t id, OnActorMeessageCallback callback);
    void onMessage(std::shared_ptr<uv::TcpConnection>connection, const char* data, ssize_t size);
    static const int HeartTimeOutSecend;

private:
    uint32_t id_;
    std::map<int, std::weak_ptr<uv::TcpConnection>> endPoints_;

private:
    OnActorMeessageCallback onActorMessageCallback_;

    void onReqFrameworkId(uint32_t id,std::shared_ptr<uv::TcpConnection> connection);
    void onActorMessage(const char* data, int size);
};
}
}
#endif // !   ORCA_ACTOR_SERVER_H

