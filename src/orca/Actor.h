#ifndef    ORCA_ACTOR_H
#define    ORCA_ACTOR_H

#include    <functional>

#include    <orca/Address.h>
#include    <orca/MessagePack.h>

namespace orca
{
namespace core
{ 
struct MessageInfo
{
    const void* enter;
    const unsigned long size;
};

class Framework;

template <typename MessageType = MESSAGE_TYPE>
class Actor
{
public:
    using ActorHandle = std::function<void(const MessagePack<MessageType>&,const Address&)>;
    Actor(Framework* framework, std::string& name = std::string(""))
        :handle_(nullptr)
    {

    }

    void setAddr()
    {

    }

    Address& Addr()
    {
        return addr_;
    }

    void registerHandler(ActorHandle handler)
    {
        handle_ = handler;
    }

private:
    Address addr_;
    ActorHandle handle_;

};
}
}
#endif // !   ORCA_ACTOR_H

