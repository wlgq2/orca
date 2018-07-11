#ifndef    ORCA_MAILBOX_CENTER
#define    ORCA_MAILBOX_CENTER

#include <vector>
#include <memory>
#include <mutex>
#include <functional>

#include   "../base/queue/BlockQueue.h"
#include "../base/SpinLock/SpinLock.h"
#include "Define.h"
#include "Mail.h"
#include "MailboxPage.h"
#include "Address.h"
#include "Assert.h"


namespace orca
{ 
namespace core
{


template <typename MailboxType,typename MailType>
class MailboxCenter
{
public:

    MailboxCenter(uint32_t id);
    ~MailboxCenter();
    using MailboxPagePtr = std::shared_ptr<MailboxPage<MailboxType>>;

    template<typename ActorType>
    int applyAdderss(ActorType*actor);

    template<typename ActorType>
    void recycle(ActorType* actor);

    template<typename MessageType>
    void sendMessage(std::shared_ptr<MessageType>& message,Address& from,Address& destination);
    template<typename MessageType>
    void sendMessage(std::shared_ptr<MessageType>& message, Address& from, std::string& name);
    
    template<typename MessageType>
    int onMessage(std::shared_ptr<MessageType>& message, Address& from, Address& destination);
    template<typename MessageType>
    int onMessage(std::shared_ptr<MessageType>& message, Address& from, std::string& name);

    int delivery();
private:
    bool applyMailboxName(std::string& name, Address& addr);
    bool getMailBoxAddr(std::string& name, Address& addr);
    bool recycleMailboxName(std::string& name);
    std::shared_ptr<MailboxType> getMailbox(const Address& addr);
    
private:
    uint32_t id_;
    orca::base::CommonLockType lock_;
    std::vector<MailboxPagePtr> mailboxs_;
    std::map<std::string, Address>  mailboxAddrs_;

    base::BlockQueue<MailType, orca::base::CommonLockType> mailCache_;
    static const int MaxPageCnt;
};

template <typename MailboxType, typename MailType>
const int MailboxCenter<MailboxType, MailType>::MaxPageCnt = 128;

template <typename MailboxType, typename MailType>
MailboxCenter<MailboxType, MailType>::MailboxCenter(uint32_t id)
    :id_(id)
{
    mailboxAddrs_.clear();
    for (auto i = 0; i < MaxPageCnt; i++)
    {
        mailboxs_.push_back(nullptr);
    }

}

template <typename MailboxType, typename MailType>
MailboxCenter<MailboxType, MailType>::~MailboxCenter()
{

}

template <typename MailboxType,typename MailType>
template<typename ActorType>
int MailboxCenter<MailboxType, MailType>::applyAdderss(ActorType* actor)
{
    std::unique_lock<orca::base::CommonLockType> lock(lock_);
    for (int i = 0; i < MaxPageCnt; i++)
    {
        if (nullptr == mailboxs_[i])
        {
            mailboxs_[i] = std::make_shared<MailboxPage<MailboxType>>();
        }
        auto index = mailboxs_[i]->allocateMailbox(std::bind(&ActorType::handle, actor, std::placeholders::_1, std::placeholders::_2));
        if (index >= 0)
        {
            actor->setAddr(id_,i, index);
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



template <typename MailboxType, typename MailType>
template<typename ActorType>
void MailboxCenter<MailboxType, MailType>::recycle(ActorType* actor)
{
    std::unique_lock<orca::base::CommonLockType> lock(lock_);
    if (mailboxs_.size() > actor->getAddress().page)
    {
        mailboxs_[actor->getAddress().page]->recycleMailbox(actor->getAddress().index);
    }
    if ("" != actor->Name())
    {
        recycleMailboxName(actor->Name());
    }
}


template<typename MailboxType, typename MailType>
template<typename MessageType>
inline void MailboxCenter<MailboxType, MailType>::sendMessage(std::shared_ptr<MessageType>& message,Address& from,Address& destination)
{
    //mailCache_.push<Address, std::shared_ptr<MessageType>>(from, destination, message);
    mailCache_.push(from, destination, message);
}

template<typename MailboxType, typename MailType>
template<typename MessageType>
inline void MailboxCenter<MailboxType, MailType>::sendMessage(std::shared_ptr<MessageType>& message, Address& from, std::string& name)
{
    Address destination;
    if (getMailBoxAddr(name, destination))
    {
        mailCache_.push(from, destination, message);
    }
    else
    {
        base::ErrorHandle::Instance()->error(base::ErrorInfo::NoFindActorName, std::string("no find actor ")+name);
    }
}

template<typename MailboxType, typename MailType>
template<typename MessageType>
inline int MailboxCenter<MailboxType, MailType>::onMessage(std::shared_ptr<MessageType>& message, Address& from, Address& destination)
{
    auto mailbox = getMailbox(destination);
    MailType mail = {from,destination,message};
    return mailbox->delivery(mail);
}

template<typename MailboxType, typename MailType>
template<typename MessageType>
inline int MailboxCenter<MailboxType, MailType>::onMessage(std::shared_ptr<MessageType>& message, Address& from, std::string& name)
{
    Address destination;

    if (getMailBoxAddr(name, destination))
    {
        auto mailbox = getMailbox(destination);
        MailType mail = { from,destination,message };
        return mailbox->delivery(mail);
    }
    else
    {
        base::ErrorHandle::Instance()->error(base::ErrorInfo::NoFindActorName, std::string("no find actor ") + name);
    }
}

template<typename MailboxType, typename MailType>
int MailboxCenter<MailboxType, MailType>::delivery()
{
    MailType mail;
    mailCache_.pop(mail);
    auto mailbox = getMailbox(mail.destination);
    return mailbox->delivery(mail);
}

template <typename MailboxType, typename MailType>
bool MailboxCenter<MailboxType, MailType>::applyMailboxName(std::string& name,Address& addr)
{
    auto it = mailboxAddrs_.find(name);
    if (it != mailboxAddrs_.end())
    {
        base::ErrorHandle::Instance()->error(base::ErrorInfo::ReDefineActorName,std::string("redefined actor name:")+name);
        return false;
    }
    mailboxAddrs_[name] = addr;
    return true;
}

template<typename MailboxType, typename MailType>
inline bool MailboxCenter<MailboxType, MailType>::getMailBoxAddr(std::string& name, Address& addr)
{
    std::unique_lock<orca::base::CommonLockType> lock(lock_);
    auto it = mailboxAddrs_.find(name);
    if (it == mailboxAddrs_.end())
        return false;
    addr = it->second;
    return true;
}

template <typename MailboxType, typename MailType>
bool MailboxCenter<MailboxType, MailType>::recycleMailboxName(std::string& name)
{
    auto it = mailboxAddrs_.find(name);
    if (it != mailboxAddrs_.end())
    {
        mailboxAddrs_.erase(it);
        return true;
    }
    return false;
}

template<typename MailboxType, typename MailType>
inline std::shared_ptr<MailboxType> MailboxCenter<MailboxType, MailType>::getMailbox(const Address& addr)
{
    std::unique_lock<orca::base::CommonLockType> lock(lock_);
    //if(addr.framework)
    if (addr.page<mailboxs_.size())
    {
        return mailboxs_[addr.page]->getMailbox(addr.index);
    }
    return nullptr;
}

}
}
#endif // !   MAILBOX_CENTER

