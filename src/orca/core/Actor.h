#ifndef    ORCA_ACTOR_H
#define    ORCA_ACTOR_H

#include    <functional>

#include    "Address.h"
#include    "MessagePack.h"

namespace orca
{
namespace core
{ 
struct MessageInfo
{
    const void* enter;
    const unsigned long size;
};
template<typename MessageType>
class Framework;

template <typename MessageType>
class Actor
{
public:
    using ActorHandle = std::function<void(const MessagePack<MessageType>&,const Address&)>;
    Actor(orca::core::Framework<MessageType>* framework, std::string name = std::string(""));
    std::string& Name();

    void setAddr(Address& addr);
    void setAddr(int page, int index);
    Address& Addr();
    void registerHandler(ActorHandle handler);
    void Handle();

private:
    std::string name_;
    Address addr_;
    ActorHandle handle_;

};

template <typename MessageType>
Actor<MessageType>::Actor(orca::core::Framework<MessageType>* framework, std::string name)
    :name_(name),
    handle_(nullptr)
{
    //addr_.framework = name_;
    framework->registerActor(this);
}

template<typename MessageType>
std::string & Actor<MessageType>::Name()
{
    return name_;
}

template <typename MessageType>
void Actor<MessageType>::setAddr(Address& addr)
{
    addr_ = addr;
}

template <typename MessageType>
void Actor<MessageType>::setAddr(int page, int index)
{
    addr_.page = page;
    addr_.index = index;
}

template <typename MessageType>
Address& Actor<MessageType>::Addr()
{
    return addr_;
}

template <typename MessageType>
void Actor<MessageType>::registerHandler(ActorHandle handler)
{
    handle_ = handler;
}

template <typename MessageType>
void Actor<MessageType>::Handle()
{
    if (handle_)
    {

    }
}
}
}
#endif // !   ORCA_ACTOR_H

