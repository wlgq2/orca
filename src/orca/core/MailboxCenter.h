#ifndef    MAILBOX_CENTER
#define    MAILBOX_CENTER

#include <vector>
#include <memory>
#include <mutex>
#include <functional>

#include   "../base/queue/BlockQueue.h"
#include "MailboxPage.h"
#include "Address.h"
#include "Assert.h"

namespace orca
{ 
namespace core
{
template<typename MessageType>
struct Mail
{
    Address addr;
    std::shared_ptr<MessageType> message;
};

template <typename MailboxType,typename QueueType>
class MailboxCenter
{
public:
    MailboxCenter();
    ~MailboxCenter();
    using MailboxPagePtr = std::shared_ptr<MailboxPage<MailboxType>>;

    template<typename ActorType>
    int applyAdderss(ActorType*actor);

    template<typename ActorType>
    void recycle(ActorType* actor);

private:
    bool applyMailboxName(std::string& name, Address& addr);
    bool recycleMailboxName(std::string& name);

    
private:
    std::mutex mutex_;
    std::vector<MailboxPagePtr> mailboxs_;
    std::map<std::string, Address>  mailboxAddrs_;

    QueueType mailCache_;
    static const int MaxPageCnt;
};

template <typename MailboxType, typename QueueType>
const int MailboxCenter<MailboxType, QueueType>::MaxPageCnt = 128;

template <typename MailboxType, typename QueueType>
MailboxCenter<MailboxType, QueueType>::MailboxCenter()
{
    mailboxAddrs_.clear();
    for (auto i = 0; i < MaxPageCnt; i++)
    {
        mailboxs_.push_back(nullptr);
    }
}

template <typename MailboxType, typename QueueType>
MailboxCenter<MailboxType, QueueType>::~MailboxCenter()
{

}

template <typename MailboxType,typename QueueType>
template<typename ActorType>
int MailboxCenter<MailboxType, QueueType>::applyAdderss(ActorType* actor)
{
    std::unique_lock<std::mutex> lock(mutex_);
    for (int i = 0; i < MaxPageCnt; i++)
    {
        if (nullptr == mailboxs_[i])
        {
            mailboxs_[i] = std::make_shared<MailboxPage<MailboxType>>();
        }
        auto index = mailboxs_[i]->allocateMailbox(std::bind(&ActorType::handle, actor, std::placeholders::_1, std::placeholders::_2));
        if (index >= 0)
        {
            actor->setAddr(i, index);
            if ("" != actor->Name())
            {
                auto rst = applyMailboxName(actor->Name(), actor->getAddress());
                ORCA_ASSERT_MSG(rst, std::string("actor name '")+ actor->Name()+"' redefine.");
            }
            return 0;
        }
    }
    return -1;
}



template <typename MailboxType, typename QueueType>
template<typename ActorType>
void MailboxCenter<MailboxType, QueueType>::recycle(ActorType* actor)
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (mailboxs_.size() > actor->getAddress().page)
    {
        mailboxs_[actor->getAddress().page]->recycleMailbox(actor->getAddress().index);
    }
    if ("" != actor->Name())
    {
        recycleMailboxName(actor->Name());
    }
}

template <typename MailboxType, typename QueueType>
bool MailboxCenter<MailboxType, QueueType>::applyMailboxName(std::string& name,Address& addr)
{
    auto it = mailboxAddrs_.find(name);
    if(it != mailboxAddrs_.end())
        return false;
    mailboxAddrs_[name] = addr;
    return true;
}

template <typename MailboxType, typename QueueType>
bool MailboxCenter<MailboxType, QueueType>::recycleMailboxName(std::string& name)
{
    auto it = mailboxAddrs_.find(name);
    if (it != mailboxAddrs_.end())
    {
        mailboxAddrs_.erase(it);
        return true;
    }
    return false;
}

}
}
#endif // !   MAILBOX_CENTER

