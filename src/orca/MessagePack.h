#ifndef      ORCA_MESSAGE_PACK_H
#define      ORCA_MESSAGE_PACK_H

#include <memory>
#include <string>

#include <orca/MessageTraits.h>
#include <orca/Define.h>

namespace orca
{
namespace core
{ 

template <typename MessageType = MESSAGE_TYPE>
class MessagePack
{
public:
    MessagePack(MessageType* ptr = nullptr)
        :messagePtr_(ptr)
    {
    }

    template<class... _Types >
    std::shared_ptr<MessageType>& create(_Types&&... _Args)
    {
        messagePtr_ = std::make_shared<MessageType>(_STD forward<_Types>(_Args)...);
        return get();
    }

    std::shared_ptr<MessageType>& get()
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
private:
    std::shared_ptr<MessageType> messagePtr_;
};
}
}
#endif 

