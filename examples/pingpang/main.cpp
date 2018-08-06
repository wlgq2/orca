#include <iostream>
#include <atomic>
#include <thread>

#include "MessageType.h"

class ActorTest :public orca::Actor
{
public:
    ActorTest(orca::Framework* framework,std::string name = "")
        :Actor(framework, name)
    {
        registerHandler(std::bind(&ActorTest::handle,this,std::placeholders::_1,std::placeholders::_2));
    }
    void handle(orca::MessagePack& pack, orca::Address& from)
    {
        cnt_++;
        send(pack, from);
    }
    void clearCnt()
    {
        cnt_ = 0;
    }
    uint64_t getCnt()
    {
        return cnt_;
    }
private:
    std::atomic<uint64_t> cnt_;
};

int main(int argc, char** args)
{
    //actor framework.
    orca::Framework framework;
    //arctor object.
    ActorTest actor1(&framework);
    ActorTest actor2(&framework);
    //message pack.
    char data[1024] = "1k ping-pang message";
    orca::MessagePack message;
    message.create(data,sizeof(data));

    //actor1->actor2 send message.
    actor1.send(message,actor2.getAddress());
    
    //new thread count.
    std::thread t1([&actor2]()
    {
        while (true)
        {
            orca::base::Thread::SleepMSeconds(1000);
            std::cout << "count: " << actor2.getCnt() << std::endl;
            actor2.clearCnt();
        }
    });
    framework.loop();

}