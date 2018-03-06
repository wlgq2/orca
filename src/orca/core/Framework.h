#ifndef    ORCA_FRAMEWORK_H
#define    ORCA_FRAMEWORK_H

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
    Framework()
    {
        //pool_.registerPorcess(std::bind(&Framework::process,this));
        //pool_.start(2);
    }

    void process()
    {
        while (true)
        {
            orca::base::Thread::SleepMSeconds(10000);
        }
    }
    

    void registerActor(ActorType* actor);
    void recycleActor(ActorType* actor);
private:
    //ThreadPool<std::queue<int>,std::function<void()>> pool_;


private:
    MailboxCenter<Mailbox<MessageType>> mailboxCenter_;
};

template<typename MessageType>
inline void Framework<MessageType>::registerActor(ActorType * actor)
{
    std::unique_lock<std::mutex> lock(mailboxCenter_.Mutex());
    auto rst = mailboxCenter_.applyAdderss(actor);
    ORCA_ASSERT_MSG(rst >= 0, "mailboxs overflow");
}

template<typename MessageType>
void Framework<MessageType>::recycleActor(ActorType * actor)
{
    std::unique_lock<std::mutex> lock(mailboxCenter_.Mutex());
    mailboxCenter_.recycle(actor);
}

}
}
#endif // ! ORCA_FRAMEWORK_H
