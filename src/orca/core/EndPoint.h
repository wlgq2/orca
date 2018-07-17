#ifndef  ORCA_CORE_ENDPOINT_H
#define  ORCA_CORE_ENDPOINT_H

#include <string>
#include <vector>
#include <queue>
#include "../base/libuv_cpp11/uv/uv11.h"
#include "../base/thread/Thread.h"
#include "net/ActorClient.h"
#include "net/ActorServer.h"
#include "RemoteMail.h"
#include "../base/error/ErrorHandle.h"
#include "Assert.h"
#include "net/Protocol.h"

namespace orca
{

namespace core
{

struct EndPointAddress
{
    enum IPV
    {
        Ipv4 = 0,
        Ipv6
    };
    EndPointAddress(std::string ip, uint16_t port, IPV ipv)
    {
        this->ip = ip;
        this->port = port;
        this->ipv = ipv;
    }
    std::string ip;
    uint16_t port;
    IPV ipv;
};

template<typename MessageType>
class EndPoint
{
public:
    using RemoteMailType = RemoteMail<MessageType>;
    using RemoteMailPtr = std::shared_ptr<RemoteMailType>;
    using OnActorMessageByName = std::function<void(Address&, std::string&, std::shared_ptr<MessageType>)>;
    using OnActorMessageByAddress = std::function<void(Address&, Address&, std::shared_ptr<MessageType>)>;

    EndPoint(EndPointAddress& addr,uint32_t id);
    ~EndPoint();

    void run();
    void appendRemoteEndPoint(struct EndPointAddress& addr);
    void clear();
    void registerRemoteMessage(OnActorMessageByName callbackByName, OnActorMessageByAddress callbackByAddress);
    void registerActorClient(uint32_t id, ActorClientPtr client);

    void send(const std::shared_ptr<MessageType> message, Address& from, Address& destination);
    void send(const std::shared_ptr<MessageType> message, Address& from, std::string& name, uint32_t framework);

    static const int MessageProcessPeriodMS;
private:
    uint32_t id_;
    uv::EventLoop loop_;
    std::atomic<bool> remoteRegisterCompleted_;
    std::vector<ActorClientPtr> endPoints_;
    std::map<uint32_t, ActorClientPtr> endPointMap_;
    std::shared_ptr<ActorServer> server_;
    uv::Timer<void*>* timer_;
    std::queue<RemoteMailPtr> sendCache_;

    //void onActorMessageCallback_;
    OnActorMessageByName onActorMessageByName_;
    OnActorMessageByAddress onActorMessageByAddress_;

    void appendMail(RemoteMailPtr mail);
    void processMail();
    void onActorMessage(const char* data, int size);
};
template<typename MessageType>
const int EndPoint<MessageType>::MessageProcessPeriodMS = 10;

template<typename MessageType>
inline EndPoint<MessageType>::EndPoint(EndPointAddress & addr, uint32_t id)
    :id_(id),
    remoteRegisterCompleted_(false),
    timer_(nullptr)
{
    uv::SocketAddr uvaddr(addr.ip, addr.port, static_cast<uv::SocketAddr::IPV>(addr.ipv));
    server_ = std::make_shared<ActorServer>(&loop_, uvaddr, id_,
        std::bind(&EndPoint<MessageType>::onActorMessage,this,std::placeholders::_1,std::placeholders::_2));
    timer_ = new uv::Timer<void*>(&loop_, MessageProcessPeriodMS, MessageProcessPeriodMS,
        [this](uv::Timer<void*>* ,void*)
    {
        processMail();
    }, nullptr);
}

template<typename MessageType>
inline EndPoint<MessageType>::~EndPoint()
{
    timer_->close(
        [](uv::Timer<void*>* timer)
    {
        //release timer pointer in loop callback.
        delete timer;
    });
}

template<typename MessageType>
inline void EndPoint<MessageType>::run()
{
    remoteRegisterCompleted_ = endPoints_.empty() ? true : false;
    for (auto it = endPoints_.begin(); it != endPoints_.end(); it++)
    {
        (*it)->connect();
    }

    server_->start();
    timer_->start();
    loop_.run();
}

template<typename MessageType>
inline void EndPoint<MessageType>::appendRemoteEndPoint(EndPointAddress & addr)
{
    uv::SocketAddr socketAddr(addr.ip, addr.port, static_cast<uv::SocketAddr::IPV>(addr.ipv));
    ActorClientPtr client = std::make_shared<ActorClient>(&loop_, socketAddr, id_);
    client->setRegisterRemoteFrameworkCallback(std::bind(&EndPoint::registerActorClient, this, std::placeholders::_1, std::placeholders::_2));
    endPoints_.push_back(client);

}

template<typename MessageType>
inline void EndPoint<MessageType>::clear()
{
    endPoints_.clear();
}

template<typename MessageType>
inline void EndPoint<MessageType>::registerRemoteMessage(OnActorMessageByName callbackByName, OnActorMessageByAddress callbackByAddress)
{
    onActorMessageByName_ = callbackByName;
    onActorMessageByAddress_ = callbackByAddress;
}

template<typename MessageType>
inline void EndPoint<MessageType>::registerActorClient(uint32_t id, ActorClientPtr client)
{
    //thread Safety
    loop_.runInThisLoop([this, client, id]()
    {
        if (id == id_)
        {
            base::ErrorHandle::Instance()->error(base::ErrorInfo::RepeatedRemoteFrameworkID, std::string("remote framework id repeated :") + std::to_string(id));
        }
        else
        {
            auto it = endPointMap_.find(id);
            if (it == endPointMap_.end())
            {
                endPointMap_[id] = client;
                if (endPointMap_.size() == endPoints_.size())
                {
                    remoteRegisterCompleted_ = true;
                }
            }
            else
            {
                //base::ErrorHandle::Instance()->error(base::ErrorInfo::RepeatedRemoteFrameworkID, std::string("remote framework id repeated :") + std::to_string(id));
            }
        }
    });
}

template<typename MessageType>
inline void EndPoint<MessageType>::send(const std::shared_ptr<MessageType> message, Address& from, Address& destination)
{
    auto ptr = std::make_shared<RemoteMail<MessageType>>(from,destination,message);
    appendMail(ptr);
}

template<typename MessageType>
inline void EndPoint<MessageType>::send(const std::shared_ptr<MessageType> message, Address& from, std::string& name, uint32_t framework)
{
    auto ptr = std::make_shared<RemoteMail<MessageType>>(from, framework, name, message);
    appendMail(ptr);

}

template<typename MessageType>
inline void EndPoint<MessageType>::appendMail(RemoteMailPtr mail)
{
    //thread safe.
    loop_.runInThisLoop([mail,this]()
    {
        sendCache_.push(mail);
    });
}

template<typename MessageType>
inline void EndPoint<MessageType>::processMail()
{
    //only run in loop thread.
    ORCA_ASSERT_MSG(loop_.isRunInLoopThread(),"remote message process can only run in uv loop thread.");
    int size = sendCache_.size();
    for (int i = 0; i < size; i++)
    {
        auto message = sendCache_.front();
        sendCache_.pop();
        uint32_t destination = message->getDestinationId();
        auto it = endPointMap_.find(destination);
        if (it != endPointMap_.end())
        {
            auto client = it->second;
            auto size = message->size();
            char* data = new char[size];
            if (0 != message->pack(data, size))
            {
                orca::base::ErrorHandle::Instance()->error(base::ErrorInfo::PackMessageError, "pack remote message error.");
                delete data;
                continue;
            }
            uv::Packet packet;
            packet.reserve_ = Protocol::ActorMessage;
            packet.fill(data, size);
            delete data;
            client->write(packet.Buffer(), packet.BufferSize(),
                [this](uv::WriteInfo& writeInfo)
            {
                if (0 != writeInfo.status)
                {
                    orca::base::ErrorHandle::Instance()->error(base::ErrorInfo::UVWriteFail, std::string("uv write message fail:")+uv::EventLoop::GetErrorMessage(writeInfo.status));
                }
            });


        }
        else
        {
            if (!remoteRegisterCompleted_)
            {
                sendCache_.push(message);
            }
            else
            {
                orca::base::ErrorHandle::Instance()->error(base::ErrorInfo::NoFindRemoteFramework, std::string("can not find remote framework:")+std::to_string(destination));
            }
        }

    }
}

template<typename MessageType>
inline void EndPoint<MessageType>::onActorMessage(const char* data, int size)
{
    RemoteMailType mail;
    mail.unpack(data, size);
    if (RemoteMailType::ByAddress == mail.getIndexMode())
    {
        if (onActorMessageByAddress_)
        {
            onActorMessageByAddress_(mail.getFromAddress(), mail.getDestinationAddress(), mail.getMessage() );
        }
    }
    else
    {
        if (onActorMessageByName_)
        {
            onActorMessageByName_(mail.getFromAddress(), mail.getDestinationActor(), mail.getMessage());
        }
    }
    processMail();
}

}
}
#endif
