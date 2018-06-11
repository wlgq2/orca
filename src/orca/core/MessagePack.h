#ifndef      ORCA_MESSAGE_PACK_H
#define      ORCA_MESSAGE_PACK_H

#include <memory>
#include <string>

#include "MessageTraits.h"
#include "Define.h"
#include "../base/error/ErrorHandle.h"

namespace orca
{
namespace core
{ 

template <typename MessageType>
class MessagePack
{
public:
    MessagePack(MessageType* ptr = nullptr)
        :messagePtr_(ptr)
    {
    }
    MessagePack(std::shared_ptr<MessageType>& ptr)
        :messagePtr_(ptr)
    {
    }
    template<class... _Types >
    std::shared_ptr<MessageType>& create(_Types&&... _Args)
    {
        messagePtr_ = std::make_shared<MessageType>(_STD forward<_Types>(_Args)...);
        return messagePtr_;
    }

    std::shared_ptr<MessageType> get() const
    {
        return messagePtr_;
    }

    const void* enter()
    {
        auto ptr = MessageTraits<MessageType>::Enter(*messagePtr_);
        return ptr;
    }
    const unsigned long size()
    {
        return messagePtr_->size();
    }
    void set(std::shared_ptr<MessageType>& ptr)
    {
        messagePtr_ = ptr;
    }
private:
    std::shared_ptr<MessageType> messagePtr_;
};
}
}
#endif 

