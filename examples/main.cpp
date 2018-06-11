#include <iostream>
#include <memory>
#include <vector>

#include <orca/orca.h>
#include <future>
#include <orca/base/libuv_cpp11/uv/uv11.h>
#include <atomic>

using namespace  std;

class MyMessage
{
public:
    MyMessage(const char* str)
    {
        memset(message, 0x00, Size);
        memcpy(message, str, strlen(str));
    }
    ~MyMessage()
    {
        //std::cout << "release" << std::endl;
    }
    const unsigned long size()
    {
        return Size;
    }

    const void* enter()
    {
        return static_cast<const void*>(message);
    }
private:
    static const unsigned long Size = 2048;
    char message[Size];
};
                                                    
REGISTER_MESSAGE_TYPE(MyMessage);
std::atomic<uint64_t> cnt;

struct test
{
    int x;
    int y;
};
class Actor1 :public orca::Actor
{
public:
    Actor1(orca::Framework* framework,std::string name = "")
        :Actor(framework, name)
    {
        registerHandler(std::bind(&Actor1::process,this,std::placeholders::_1,std::placeholders::_2));
    }
    void process(const orca::MessagePack& pack, orca::Address& from)
    {
        std::cout << (char*)(pack.get()->enter()) << std::endl;
        cnt++;
        //Sleep(1);
        //send(pack, from);
    }
};

int main(int argc, char** args)
{
    std::shared_ptr<int> dfd;
    std::cout << sizeof(dfd) << std::endl;
    orca::EndPoint endpoint("0.0.0.0", 10002);
    endpoint.appendRemoteEndPoint("127.0.0.1", 10001);

    orca::Framework::RegisterErrorHandle([](orca::base::ErrorInfo errorInfo)
    {
        std::cout << "error " << errorInfo.getErrorId() << ":" << errorInfo.getErrorInfo()<< std::endl;
    });
    orca::Framework framework(&endpoint,1);

    orca::MessagePack message;
    message.create("a message from actor1");
    Actor1 actor(&framework);
    Actor1 actor2(&framework,"actor02");

    //actor.send(message, actor2.getAddress());
    actor.send(message, "actor03");
    //actor.send(message, actor.getAddress());
    //actor.send(message, actor.getAddress());
    //actor.send(message, actor.getAddress());
    //actor.send(message, actor.getAddress());
    auto async = std::async([&actor,&message]()
    {
        while (true)
        {
            Sleep(1000);
            if (cnt == 0)
            {
                //exit(0);
            }
            //actor.send(message, actor.getAddress());
            std::cout << cnt << std::endl;
            cnt = 0;
        }
    });
    framework.loop();

}