#ifndef    MAILBOX_CENTER
#define    MAILBOX_CENTER

#include <vector>
#include <memory>

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
    void recycle(Address& addr);

private:
    std::vector<MailboxPagePtr> mailboxs_;
    std::map<std::string, Address>  mailboxIndex_;

    static const int MaxPageCnt;
};

template <typename MailboxType>
const int MailboxCenter<MailboxType>::MaxPageCnt = 128;

template<typename MailboxType>
MailboxCenter<MailboxType>::MailboxCenter()
{
    mailboxs_.clear();
    mailboxIndex_.clear();
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
        if (mailboxs_.size() < i + 1)
        {
            MailboxPagePtr page = std::make_shared<MailboxPage<MailboxType>>();
            mailboxs_.push_back(page);
        }
        auto index = mailboxs_[i]->allocateMailbox(actor);
        if (index >= 0)
        {
            actor->setAddr(i,index);
            return 0;
        }
    }
    return -1;
}



template<typename MailboxType>
void MailboxCenter<MailboxType>::recycle(Address& addr)
{
    if (mailboxs_.size() > addr.page)
    {
        mailboxs_[addr.page]->
    }
}

}
}
#endif // !   MAILBOX_CENTER

