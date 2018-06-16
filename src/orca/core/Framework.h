/*
Copyright 2017, object_he@yeah.net  All rights reserved.

Author: object_he@yeah.net

Last modified: 2018-4-7

Description:
*/

#ifndef    ORCA_FRAMEWORK_H
#define    ORCA_FRAMEWORK_H

#include   <memory>
#include   "../base/thread/ThreadPool.h"
#include   "net/EndPoint.h"

#include   "Actor.h"
#include   "Mailbox.h"
#include   "MailboxCenter.h"

namespace orca
{
namespace core
{ 

template<typename MessageType>
class Framework
{
public:
    using ActorType = Actor<MessageType>;
    using MessagePtr = std::shared_ptr<MessageType>;
    using MailType = Mail<MessageType>;
    using MailboxType = Mailbox<MessageType>;

    Framework(EndPointAddress* endPointAddr, uint32_t id);
    Framework();

    void process();

    void registerActor(ActorType* actor);
    void recycleActor(ActorType* actor);
    
    void send(const MessagePack<MessageType>& message,Address& from,Address& destination);
    void send(const MessagePack<MessageType>& message, Address& from,std::string& name,uint32_t framework);

    void appendRemoteEndPoint(struct EndPointAddress& addr);
    void appendRemoteEndPoint(std::string ip, uint16_t port, EndPointAddress::IPV ipv = EndPointAddress::Ipv4);
    uint64_t getID();
    void loop();

public:
    static void RegisterErrorHandle(base::ErrorHandle::ErrorHandleFunction callback);
private:
    orca::base::ThreadPool threadPool_;
    const uint32_t uniqueID_;

private:
    MailboxCenter<MailboxType, MailType> mailboxCenter_;
    std::shared_ptr<EndPoint> endPoint_;


};



template<typename MessageType>
Framework<MessageType>::Framework(EndPointAddress* endPointAddr, uint32_t id)
    :uniqueID_(id),
    mailboxCenter_(id),
    endPoint_(nullptr)
{
    if (nullptr != endPointAddr)
    {
        endPoint_ = std::make_shared<EndPoint>(*endPointAddr, uniqueID_);
    }
    threadPool_.registerPorcess(std::bind(&Framework::process, this));
}

template<typename MessageType>
Framework<MessageType>::Framework()
    :Framework(nullptr,0)
{
}

template<typename MessageType>
void Framework<MessageType>::process()
{
    mailboxCenter_.delivery();
}

template<typename MessageType>
inline void Framework<MessageType>::registerActor(ActorType* actor)
{
    auto rst = mailboxCenter_.applyAdderss(actor);
    ORCA_ASSERT_MSG(rst >= 0, "mailboxs overflow");
}

template<typename MessageType>
void Framework<MessageType>::recycleActor(ActorType * actor)
{
    mailboxCenter_.recycle(actor);
}

template<typename MessageType>
inline void Framework<MessageType>::send(const MessagePack<MessageType>& message,Address& from,Address& destination)
{
    //local message.
    if (destination.framework == uniqueID_)
    {
        mailboxCenter_.sendMessage(message.get(), from, destination);
    }
    else //romote message.
    {

    }
}

template<typename MessageType>
inline void Framework<MessageType>::send(const MessagePack<MessageType>& message, Address& from, std::string& name, uint32_t framework)
{
    //local message.
    if (uniqueID_ == framework)
    {
        mailboxCenter_.sendMessage(message.get(), from, name);
    }
    else //romote message.
    {
        
    }
}

template<typename MessageType>
inline void Framework<MessageType>::appendRemoteEndPoint(EndPointAddress& addr)
{
    if (nullptr != endPoint_)
    {
        endPoint_->appendRemoteEndPoint(addr);
    }
}

template<typename MessageType>
inline void Framework<MessageType>::appendRemoteEndPoint(std::string ip, uint16_t port, EndPointAddress::IPV ipv)
{
    EndPointAddress addr = { ip,port,ipv };
    appendRemoteEndPoint(addr);
}


template<typename MessageType>
inline uint64_t Framework<MessageType>::getID()
{
    return uniqueID_;
}

template<typename MessageType>
void Framework<MessageType>::loop()
{
    threadPool_.start(1);
    if (endPoint_)
        endPoint_->run();
    threadPool_.join();
}
template<typename MessageType>
inline void Framework<MessageType>::RegisterErrorHandle(base::ErrorHandle::ErrorHandleFunction callback)
{
    base::ErrorHandle::Instance()->bind(callback);
}
}
}
#endif // ! ORCA_FRAMEWORK_H
