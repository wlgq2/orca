#ifndef  ORCA_CORE_REMOTE_MAIL_H
#define  ORCA_CORE_REMOTE_MAIL_H

#include <memory>
#include "Address.h"

namespace orca
{ 
namespace core
{

struct RemoteActorName
{
    uint32_t frameworkId;
    std::string actorName;
};

template<typename MessageType>
class RemoteMail
{
public:
    enum IndexMode
    {
        ByAddress = 1,
        ByName
    };
    RemoteMail(struct Address& from, struct Address& to, std::shared_ptr<MessageType> message);
    RemoteMail(struct Address& from, uint32_t framework ,std::string& name, std::shared_ptr<MessageType> message);
    int unpack(char* data, int size);
    int pack(char* data, int size);
    int size();
private:
    IndexMode indexMode_;
    struct Address from_;
    struct Address to_;
    RemoteActorName remoteActor_;
    std::shared_ptr<MessageType> message_;

    int packAddress(struct Address& addr,char* data);
    int packString(std::string& name, char* data);

    int extendSize();
};


template<typename MessageType>
inline RemoteMail<MessageType>::RemoteMail(struct Address& from, struct Address& to, std::shared_ptr<MessageType> message)
    : indexMode_(ByAddress),
    from_(from),
    to_(to),
    name_(""),
    message_(message)
{
}

template<typename MessageType>
inline RemoteMail<MessageType>::RemoteMail(struct Address& from, uint32_t framework, std::string& name, std::shared_ptr<MessageType> message)
    : indexMode_(ByName),
    from_(from),
    remoteActor_({framework,name}),
    message_(message)
{
}

template<typename MessageType>
inline int RemoteMail<MessageType>::unpack(char* data, int size)
{

    return 0;
}

template<typename MessageType>
inline int RemoteMail<MessageType>::pack(char* data, int size)
{
    if (size < this->size())
    {
        return -1;
    }
    int index = 0;
    data[index++] = indexMode_;
    index += packAddress(from_,&data[index]);
    if (indexMode_ == ByAddress)
    {
        index += packAddress(to_, &data[index]);
    }
    else
    {
        index += packString(name_, &data[index]);
    }
    
    return 0;
}

template<typename MessageType>
inline int RemoteMail<MessageType>::size()
{
    return extendSize() + message_->size();
}


template<typename MessageType>
inline int RemoteMail<MessageType>::packAddress(struct Address& addr, char* data)
{
    char* p = (char*)(&from_);
    int addrsize = sizeof(from_);
    std::copy(p, p + addrsize, data);
    return addrsize;
}

template<typename MessageType>
inline int RemoteMail<MessageType>::packString(std::string& name, char* data)
{
    int strSize = name_.size();
    if (strSize > 255)
    {
        base::ErrorHandle::Instance()->error(base::ErrorInfo::ActorNameTooLong, std::string("actor's name more than 512 bytes:") + name_);
        return -1;
    }
    data[0] = strSize;
    const char* p = name_.c_str();
    std::copy(p, p + strSize, &data[1]);
    return strSize+1;
}

template<typename MessageType>
inline int RemoteMail<MessageType>::extendSize()
{
    int size = 0;
    if (indexMode_ == ByName)
    {
        size = sizeof(char) +sizeof(from_) +sizeof(char) +name_.size() ;
    }
    else
    {
        size = sizeof(char) + sizeof(from_) + sizeof(to_) ;
    }
    return size;
}

}
}
#endif // ! ORCA_CORE_REMOTE_MESSAGE_H

