#ifndef    MAILBOX_CENTER
#define    MAILBOX_CENTER

#include <vector>
#include <memory>
#include <mutex>

#include "MailboxPage.h"
#include "Address.h"
#include "Assert.h"

namespace orca
{ 
namespace core
{

template <typename MailboxType>
class MailboxCenter
{
public:
    MailboxCenter();
    ~MailboxCenter();
    using MailboxPagePtr = std::shared_ptr<MailboxPage<MailboxType>>;

    template<typename ActorType>
    int applyAdderss(ActorType*);
    template<typename ActorType>
    void recycle(ActorType* actor);

    bool applyMailboxName(std::string& name, Address& addr);
    bool recycleMailboxName(std::string& name);

    std::mutex& Mutex();
private:
    std::mutex mutex_;
    std::vector<MailboxPagePtr> mailboxs_;
    std::map<std::string, Address>  mailboxIndex_;

    static const int MaxPageCnt;
};

template <typename MailboxType>
const int MailboxCenter<MailboxType>::MaxPageCnt = 128;

template<typename MailboxType>
MailboxCenter<MailboxType>::MailboxCenter()
{
    mailboxIndex_.clear();
    for (auto i = 0; i < MaxPageCnt; i++)
    {
        mailboxs_.push_back(nullptr);
    }
}

template<typename MailboxType>
MailboxCenter<MailboxType>::~MailboxCenter()
{

}

template<typename MailboxType>
template<typename ActorType>
int MailboxCenter<MailboxType>::applyAdderss(ActorType* actor)
{
    for (int i = 0; i < MaxPageCnt; i++)
    {
        if (nullptr == mailboxs_[i])
        {
            mailboxs_[i] = std::make_shared<MailboxPage<MailboxType>>();
        }
        auto index = mailboxs_[i]->allocateMailbox(actor);
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



template<typename MailboxType>
template<typename ActorType>
void MailboxCenter<MailboxType>::recycle(ActorType* actor)
{
    if (mailboxs_.size() > actor->getAddress().page)
    {
        mailboxs_[actor->getAddress().page]->recycleMailbox(actor->getAddress().index);
    }
    if ("" != actor->Name())
    {
        recycleMailboxName(actor->Name());
    }
}

template<typename MailboxType>
bool MailboxCenter<MailboxType>::applyMailboxName(std::string& name,Address& addr)
{
    auto it = mailboxIndex_.find(name);
    if(it != mailboxIndex_.end())
        return false;
    mailboxIndex_[name] = addr;
    return true;
}

template<typename MailboxType>
bool MailboxCenter<MailboxType>::recycleMailboxName(std::string& name)
{
    auto it = mailboxIndex_.find(name);
    if (it != mailboxIndex_.end())
    {
        mailboxIndex_.erase(it);
        return true;
    }
    return false;
}

template<typename MailboxType>
std::mutex & MailboxCenter<MailboxType>::Mutex()
{
    return mutex_;
}

}
}
#endif // !   MAILBOX_CENTER

