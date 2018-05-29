#include "ActorServer.h"

using namespace orca::core;
using namespace std;

ActorServer::ActorServer(uv::EventLoop* loop, uv::SocketAddr& addr)
    :uv::TcpServer(loop,addr)
{
}
