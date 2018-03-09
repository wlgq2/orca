#ifndef  MAILBOX_H
#define  MAILBOX_H

#include <memory> 
#include <functional>

#include "MessagePack.h"
#include "Address.h"

namespace orca
{

namespace core
{

template  <typename MessageType>
class Mailbox
{
public:
    using MailboxHandler = std::function<void(const MessagePack<MessageType>&, const Address&)>;
    Mailbox(MailboxHandler handler);
    int handle(const MessagePack<MessageType>& message, const Address& addr);
private:
    MailboxHandler handler_;
};

template  <typename MessageType>
Mailbox<MessageType>::Mailbox(MailboxHandler handler)
    :handler_(handler)
{

}

template  <typename MessageType>
int Mailbox<MessageType>::handle(const MessagePack<MessageType>& message, const Address& addr)
{
    if (handler_)
    {
        handler_(message,addr);
    }
}

}
}
#endif // ! MAILBOX_H

