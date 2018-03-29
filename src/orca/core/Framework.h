#ifndef    ORCA_FRAMEWORK_H
#define    ORCA_FRAMEWORK_H

#include   <memory>
#include   "../base/thread/ThreadPool.h"
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

    Framework();

    void process();

    void registerActor(ActorType* actor);
    void recycleActor(ActorType* actor);
    
    void send(const MessagePack<MessageType>& message,Address& from,Address& destination);

    void loop();
private:
    orca::base::ThreadPool threadPool_;


private:
    MailboxCenter<Mailbox<MessageType>, orca::base::BlockQueue<MailType>> mailboxCenter_;
};



template<typename MessageType>
Framework<MessageType>::Framework()
{
    threadPool_.registerPorcess(std::bind(&Framework::process, this));
}

template<typename MessageType>
void Framework<MessageType>::process()
{
    mailboxCenter_.delivery();
}

template<typename MessageType>
inline void Framework<MessageType>::registerActor(ActorType * actor)
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
void Framework<MessageType>::send(const MessagePack<MessageType>& message,Address& from,Address& destination)
{
    mailboxCenter_.sendMessage(message.get(),from,destination);
}

template<typename MessageType>
void Framework<MessageType>::loop()
{
    threadPool_.start(2);
    threadPool_.join();
}
}
}
#endif // ! ORCA_FRAMEWORK_H
