#ifndef  MAILBOX_H
#define  MAILBOX_H

#include <memory> 
#include "Actor.h"

namespace orca
{

namespace core
{

template  <typename MessageType>
class Mailbox
{
public:
    Mailbox(Actor<MessageType>* actor);
    int handle();
private:
    Actor<MessageType>* actor_;
};

template  <typename MessageType>
Mailbox<MessageType>::Mailbox(Actor<MessageType>* actor)
    :actor_(actor)
{

}

template  <typename MessageType>
int Mailbox<MessageType>::handle()
{
    if (actor_)
    {

    }
}

}
}
#endif // ! MAILBOX_H

