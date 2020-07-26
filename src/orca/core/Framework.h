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
class UvConfig
{
public:
    static UvConfig& RunOnce()
    {
        static UvConfig single;
        return single;
    }
private:
    UvConfig()
    {
        uv::GlobalConfig::BufferModeStatus = uv::GlobalConfig::CycleBuffer;
        uv::GlobalConfig::CycleBufferSize = 1024 << 6; //64kb
        uv::GlobalConfig::ReadBufferVoid = std::bind(&TcpPacket::ReadTcpBuffer, std::placeholders::_1, std::placeholders::_2);
    }
};
struct FrameworkConfig
{
    FrameworkConfig()
    {
        reset();
    }
    void reset()
    {
        id = 0;
        threadCount = 1;
        endPointAddress = nullptr;
    }
    uint32_t id;
    uint32_t threadCount;
    std::shared_ptr<struct EndPointAddress> endPointAddress;

};

template<typename MessageType>
class Framework
{
public:
    using ActorType = Actor<MessageType>;
    using MessagePtr = std::shared_ptr<MessageType>;
    using MailType = Mail<MessageType>;
    using MailboxType = Mailbox<MessageType>;

    Framework(struct FrameworkConfig& config);
    Framework();

    void process();

    void registerActor(ActorType* actor);
    void recycleActor(ActorType* actor);

    void send(const MessagePack<MessageType>& message,Address& from,Address& destination);
    void send(const MessagePack<MessageType>& message, Address& from,std::string& name,uint32_t framework);
    void onRemoteMessageByName(Address& from, std::string& name, std::shared_ptr<MessageType> message);
    void onRemoteMessageByAddress(Address& from, Address& name, std::shared_ptr<MessageType> message);

    void appendRemoteEndPoint(struct EndPointAddress& addr);
    void appendRemoteEndPoint(std::string ip, uint16_t port, EndPointAddress::IPV ipv = EndPointAddress::Ipv4);
    uint64_t getID();
    void loop();

public:
    static void RegisterErrorHandle(base::ErrorHandle::ErrorHandleFunction callback);
private:
    const uint32_t uniqueID_;
    uint32_t threadCount_;
    orca::base::ThreadPool threadPool_;

private:
    MailboxCenter<MailboxType, MailType> mailboxCenter_;
    std::shared_ptr<EndPoint<MessageType>> endPoint_;

    bool checkMessage(const MessagePack<MessageType>& message);

};



template<typename MessageType>
Framework<MessageType>::Framework(struct FrameworkConfig& config)
    :uniqueID_(config.id),
    threadCount_(config.threadCount),
    mailboxCenter_(config.id),
    endPoint_(nullptr)
{
    if (nullptr != config.endPointAddress)
    {
        endPoint_ = std::make_shared<EndPoint<MessageType>>(*(config.endPointAddress), uniqueID_);
        endPoint_->registerRemoteMessage(std::bind(&Framework<MessageType>::onRemoteMessageByName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&Framework<MessageType>::onRemoteMessageByAddress,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    }
    UvConfig::RunOnce();
    threadPool_.registerPorcess(std::bind(&Framework::process, this));
}

template<typename MessageType>
Framework<MessageType>::Framework()
    :uniqueID_(0),
    threadCount_(1),
    mailboxCenter_(0),
    endPoint_(nullptr)
{
    UvConfig::RunOnce();
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
    if (!checkMessage(message))
        return;
    //local message.
    if (destination.framework == uniqueID_)
    {
        mailboxCenter_.sendMessage(message.get(), from, destination);
    }
    else //romote message.
    {
        if (endPoint_)
            endPoint_->send(message.get(), from, destination);
    }
}

template<typename MessageType>
inline void Framework<MessageType>::send(const MessagePack<MessageType>& message, Address& from, std::string& name, uint32_t framework)
{
    if (!checkMessage(message))
        return;
    //local message.
    if (uniqueID_ == framework)
    {
        mailboxCenter_.sendMessage(message.get(), from, name);
    }
    else //romote message.
    {
        if (endPoint_)
            endPoint_->send(message.get(), from, name,framework);
    }
}

template<typename MessageType>
inline void Framework<MessageType>::onRemoteMessageByName(Address& from, std::string& name, std::shared_ptr<MessageType> message)
{
    mailboxCenter_.onMessage(message, from, name);
}

template<typename MessageType>
inline void Framework<MessageType>::onRemoteMessageByAddress(Address& from, Address& to, std::shared_ptr<MessageType> message)
{
    mailboxCenter_.onMessage(message, from, to);
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
    threadPool_.start(threadCount_);
    if (endPoint_)
        endPoint_->run();
    threadPool_.join();
}

template<typename MessageType>
inline void Framework<MessageType>::RegisterErrorHandle(base::ErrorHandle::ErrorHandleFunction callback)
{
    base::ErrorHandle::Instance()->bind(callback);
}

template<typename MessageType>
inline bool Framework<MessageType>::checkMessage(const MessagePack<MessageType>& message)
{
    auto rst = (nullptr != message.get());
    if (!rst)
    {
        base::ErrorHandle::Instance()->error(base::ErrorInfo::MessagePackNull, "send a null message");
    }
    return rst;
}
}
}
#endif // ! ORCA_FRAMEWORK_H
