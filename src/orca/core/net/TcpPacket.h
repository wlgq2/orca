/*
Copyright 2017, orcaer@yeah.net  All rights reserved.

Author: orcaer@yeah.net

Last modified: 2020-7-26

Description:
*/
#ifndef    ORCA_TCP_PACKET_H
#define    ORCA_TCP_PACKET_H

#include  "../../base/uv-cpp/uv/include/uv11.h"

namespace orca
{
namespace core
{
class TcpPacket :public uv::Packet
{
public :
    void packWithType(const char* data, uint16_t size);
    const char* getData();
    static int ReadTcpBuffer(uv::PacketBuffer* packetbuf, void* out);
public:
    uint64_t messageType_;
};

}
}
#endif // !   ORCA_ACTOR_SERVER_H

