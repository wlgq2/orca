#include "EndPoint.h"

using namespace orca::core;

orca::core::EndPoint::EndPoint(std::string ip, uint16_t port)
{
}

void orca::core::EndPoint::run()
{
    loop_.run();
}
