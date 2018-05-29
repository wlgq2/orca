#ifndef    ORCA_ACTOR_SERVER_H
#define    ORCA_ACTOR_SERVER_H

#include  "../../base/libuv_cpp11/uv/uv11.h"

namespace orca
{

namespace core
{

class ActorServer : public uv::TcpServer
{
public:
    ActorServer(uv::EventLoop* loop,uv::SocketAddr& addr);
};
}
}
#endif // !   ORCA_ACTOR_SERVER_H

