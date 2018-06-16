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

struct EndPointAddress
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
    EndPoint(EndPointAddress& addr,uint32_t id);
    void run();
    void appendRemoteEndPoint(struct EndPointAddress& addr);
    void clear();

    void registerActorClient(uint32_t id, ActorClientPtr client);
private:
    uint32_t id_;
    uv::EventLoop loop_;
    std::atomic<bool> remoteRegisterCompleted_;
    std::vector<ActorClientPtr> endPoints_;
    std::map<uint32_t, ActorClientPtr> endPointMap_;
    std::shared_ptr<ActorServer> server_;

};
}
}
#endif
