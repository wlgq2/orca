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
    using MailType = Mail<std::shared_ptr<MessageType>>;

    Framework()
    {
        //pool_.registerPorcess(std::bind(&Framework::process,this));
        //pool_.start(2);
    }

    void process()
    {
        while (true)
        {
            mailboxCenter_.delivery();
            //orca::base::Thread::SleepMSeconds(10000);
        }
    }
    

    void registerActor(ActorType* actor);
    void recycleActor(ActorType* actor);

    void send(std::shared_ptr<MessageType> message, Address& from, Address& destination);
private:
    //ThreadPool<std::queue<int>,std::function<void()>> pool_;


private:
    MailboxCenter<Mailbox<MessageType>, orca::base::BlockQueue<MailType>> mailboxCenter_;
};

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
void Framework<MessageType>::send(std::shared_ptr<MessageType> message, Address & from, Address & destination)
{
    mailboxCenter_.sendMessage(message,from,destination);
}

}
}
#endif // ! ORCA_FRAMEWORK_H
