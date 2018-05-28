#ifndef  ORCA_CORE_ENDPOINT_H
#define  ORCA_CORE_ENDPOINT_H

#include <string>
#include "../base/libuv_cpp11/uv/uv11.h"
#include "../base/thread/Thread.h"

namespace orca
{

namespace core
{

class EndPoint
{
public:
    EndPoint(std::string ip,uint16_t port);

    void run();
private:
    uv::EventLoop loop_;
    //base::Thread thread_;
};
}
}
#endif
