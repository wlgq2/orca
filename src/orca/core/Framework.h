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
#include   "EndPoint.h"

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

    Framework(EndPoint* endpoint = nullptr, uint64_t id =0);

    void process();

    void registerActor(ActorType* actor);
    void recycleActor(ActorType* actor);
    
    void send(const MessagePack<MessageType>& message,Address& from,Address& destination);
    void send(const MessagePack<MessageType>& message, Address& from,std::string& name);

    uint64_t getID();
    void loop();

public:
    static void RegisterErrorHandle(ErrorHandle::ErrorHandleFunction callback);
private:
    orca::base::ThreadPool threadPool_;
    const uint64_t uniqueID_;

private:
    MailboxCenter<MailboxType, MailType> mailboxCenter_;
    EndPoint* endPoint_;


};



template<typename MessageType>
Framework<MessageType>::Framework(EndPoint* endpoint, uint64_t id)
    :uniqueID_(id),
    mailboxCenter_(id),
    endPoint_(endpoint)
{
    threadPool_.registerPorcess(std::bind(&Framework::process, this));
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
    mailboxCenter_.sendMessage(message.get(),from,destination);
}

template<typename MessageType>
inline void Framework<MessageType>::send(const MessagePack<MessageType>& message, Address& from, std::string& name)
{
    mailboxCenter_.sendMessage(message.get(), from, name);
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
inline void Framework<MessageType>::RegisterErrorHandle(ErrorHandle::ErrorHandleFunction callback)
{
    ErrorHandle::Instance()->bind(callback);
}
}
}
#endif // ! ORCA_FRAMEWORK_H
