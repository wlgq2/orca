/*
Copyright 2017, object_he@yeah.net  All rights reserved.

Author: object_he@yeah.net

Last modified: 2018-4-7

Description:
*/
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
    using ActorHandle = std::function<void(MessagePack<MessageType>&, Address&)>;
    Actor(orca::core::Framework<MessageType>* framework, std::string name = std::string(""));
    ~Actor();
    std::string& Name();

    void setAddr(Address& addr);
    void setAddr(uint32_t framework,int page, int index);
    Address& getAddress();
    void registerHandler(ActorHandle handler);
    void handle(MessagePack<MessageType>&,Address&);

    void send(const MessagePack<MessageType>& message,Address& destination);
    void send(const MessagePack<MessageType>& message, std::string& name, uint32_t framework);
    void send(const MessagePack<MessageType>& message, std::string&& name, uint32_t framework);
    void send(const MessagePack<MessageType>& message, std::string& name);
    void send(const MessagePack<MessageType>& message, std::string&& name);
    Framework<MessageType>* framework();
private:
    std::string name_;
    Framework<MessageType>* framework_;
    Address addr_;
    ActorHandle handle_;

};

template <typename MessageType>
Actor<MessageType>::Actor(orca::core::Framework<MessageType>* framework, std::string name)
    :name_(name),
    framework_(framework),
    handle_(nullptr)
{
    //addr_.framework = name_;
    
    framework_->registerActor(this);
}

template<typename MessageType>
inline Actor<MessageType>::~Actor()
{
    framework_->recycleActor(this);
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
void Actor<MessageType>::setAddr(uint32_t framework, int page, int index)
{
    addr_.framework = framework;
    addr_.page = page;
    addr_.index = index;
}

template <typename MessageType>
Address& Actor<MessageType>::getAddress()
{
    return addr_;
}

template <typename MessageType>
void Actor<MessageType>::registerHandler(ActorHandle handler)
{
    handle_ = handler;
}

template <typename MessageType>
void Actor<MessageType>::handle(MessagePack<MessageType>& message,Address& addr)
{
    if (handle_)
    {
        handle_(message, addr);
    }
}

template<typename MessageType>
inline void Actor<MessageType>::send(const MessagePack<MessageType>& message,Address& destination)
{
    framework_->send(message,addr_, destination);
}

template<typename MessageType>
inline void Actor<MessageType>::send(const MessagePack<MessageType>& message, std::string & name, uint32_t framework)
{
    framework_->send(message, addr_, name, framework);
}

template<typename MessageType>
inline void Actor<MessageType>::send(const MessagePack<MessageType>& message, std::string && name, uint32_t framework)
{
    framework_->send(message, addr_, name, framework);
}

template<typename MessageType>
inline void Actor<MessageType>::send(const MessagePack<MessageType>& message, std::string& name)
{
    send(message, name, addr_.framework);
}

template<typename MessageType>
inline void Actor<MessageType>::send(const MessagePack<MessageType>& message, std::string&& name)
{
    send(message, name, addr_.framework);
}

template<typename MessageType>
inline Framework<MessageType>* Actor<MessageType>::framework()
{
    return framework_;
}

}
}
#endif // !   ORCA_ACTOR_H

