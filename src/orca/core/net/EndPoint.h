#ifndef  ORCA_CORE_ENDPOINT_H
#define  ORCA_CORE_ENDPOINT_H

#include <string>
#include <vector>
#include "../../base/libuv_cpp11/uv/uv11.h"
#include "../../base/thread/Thread.h"
#include "ActorClient.h"
#include "ActorServer.h"

namespace orca
{

namespace core
{

struct EndPointAddr
{
    enum IPV
    {
        Ipv4 = 0,
        Ipv6
    };
    std::string ip;
    uint16_t port;
    IPV ipv;
};

class EndPoint
{
public:
    EndPoint(std::string ip,uint16_t port, EndPointAddr::IPV ipv = EndPointAddr::Ipv4);

    void run();
    void appendRemoteEndPoint(struct EndPointAddr& addr);
    void appendRemoteEndPoint(std::string ip, uint16_t port, EndPointAddr::IPV ipv = EndPointAddr::Ipv4);
    void clear();

private:
    uv::EventLoop loop_;
    std::vector<ActorClientPtr> endPoints_;
    std::shared_ptr<ActorServer> server_;
};
}
}
#endif
