#ifndef  MAILBOX_PAGE_H
#define  MAILBOX_PAGE_H

#include <map>
#include <set>
#include <memory>

#define   ORCA_MAILBOX_PAGE_SIZE   1024

namespace orca
{
namespace core
{

template <typename MailboxType>
class MailboxPage
{
public:
    using MailboxTypePtr = std::shared_ptr<MailboxType>;
    MailboxPage();
    ~MailboxPage();

    template<class... _Types >
    int allocateMailbox(_Types&&... _Args);
    void recycleMailbox(int index);

    MailboxTypePtr getMailbox(int index);
    
private:
    MailboxTypePtr mailboxs_[ORCA_MAILBOX_PAGE_SIZE];
    
    std::set<int> idleIndexs_;
};


template <typename MailboxType>
MailboxPage<MailboxType>::MailboxPage()
{
    for (int i = 0; i < ORCA_MAILBOX_PAGE_SIZE; i++)
    {
        idleIndexs_.insert(i);
        mailboxs_[i]=nullptr;
    }
}

template <typename MailboxType>
MailboxPage<MailboxType>::~MailboxPage()
{
    idleIndexs_.clear();
}

template<typename MailboxType>
template<class... _Types >
int MailboxPage<MailboxType>::allocateMailbox(_Types&&... _Args)
{
    if (idleIndexs_.empty())
        return -1;
    auto it = idleIndexs_.begin();
    int index = *it;
    idleIndexs_.erase(it);
    mailboxs_[index] = std::make_shared<MailboxType>(_STD forward<_Types>(_Args)...);
    return index;
}

template<typename MailboxType>
void MailboxPage<MailboxType>::recycleMailbox(int index)
{
    if (index < ORCA_MAILBOX_PAGE_SIZE)
    {
        idleIndexs_.insert(index);
        mailboxs_[index] = nullptr;
    }
}

template <typename MailboxType>
std::shared_ptr<MailboxType> MailboxPage<MailboxType>::getMailbox(int index)
{
    return mailboxs_[index];
}




}
}
#endif // ! MAILBOX_PAGE_H

