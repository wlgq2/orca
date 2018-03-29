#ifndef  ORCA_MAIL_H
#define  ORCA_MAIL_H

#include <memory>

namespace orca
{

namespace core
{

template<typename MessageType>
struct Mail
{
    Address from;
    Address destination;
    std::shared_ptr<MessageType> message;
};
}
}
#endif // ! ORCA_MAIL_H

