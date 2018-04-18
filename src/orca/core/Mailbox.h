#ifndef  ORCA_MAILBOX_H
#define  ORCA_MAILBOX_H

#include <memory> 
#include <functional>

#include "Mail.h"
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
    using MailboxHandler = std::function<void(const MessagePack<MessageType>&, Address&)>;
    Mailbox(MailboxHandler handler);
    int delivery(MessagePack<MessageType>& message, Address& addr);
    int delivery(Mail<MessageType>& mail);
private:
    MailboxHandler handler_;
};

template  <typename MessageType>
Mailbox<MessageType>::Mailbox(MailboxHandler handler)
    :handler_(handler)
{

}

template  <typename MessageType>
inline int Mailbox<MessageType>::delivery(MessagePack<MessageType>& message, Address& addr)
{
    if (handler_)
    {
        handler_(message,addr);
        return 0;
    }
    return -1;
}

template<typename MessageType>
inline int Mailbox<MessageType>::delivery(Mail<MessageType>& mail)
{
    MessagePack<MessageType> pack(mail.message);
    return delivery(pack,mail.from);
}

}
}
#endif // ! MAILBOX_H

