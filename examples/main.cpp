#include <iostream>
#include <memory>
#include <vector>

#include <orca/orca.h>
#include <future>
#include <chrono>
#include <thread>
#include <orca/base/libuv_cpp11/uv/uv11.h>
#include <atomic>

using namespace  std;

class MyMessage
{
public:
    MyMessage(char* str,int size)
    {
        if (size > Size)
            size = Size;
        memcpy(message, str, size);
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
    static const int Size = 2048;
    char message[Size];
};

//REGISTER_MESSAGE_TYPE(MyMessage);
REGISTER_MESSAGE_TYPE(std::string);
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
    void process(orca::MessagePack& pack, orca::Address& from)
    {
        std::cout << (char*)(pack.enter()) << endl;
        cnt++;
        //Sleep(1);
        //send(pack, from);
    }
};

int main(int argc, char** args)
{
    orca::Framework::RegisterErrorHandle([](orca::base::ErrorInfo errorInfo)
    {
        std::cout << "error " << errorInfo.getErrorId() << ":" << errorInfo.getErrorInfo()<< std::endl;
    });

    orca::FrameworkConfig config;
    config.endPointAddress = std::make_shared<orca::EndPointAddress>( "0.0.0.0",10001 ,orca::core::EndPointAddress::Ipv4 );
    config.threadCount = 1;
    config.id = 231;

    orca::Framework framework(config);
    framework.appendRemoteEndPoint("127.0.0.1", 10002);

    char str[] = "a message from actor1";
    orca::MessagePack message;
    message.create(str,sizeof(str));
    std::cout << (char*)message.enter() << endl;
    Actor1 actor(&framework);
    Actor1 actor2(&framework,"actor02");

    //actor.send(message, actor2.getAddress());
    actor.send(message, "actor02");
    //actor.send(message, actor.getAddress());
    //actor.send(message, actor.getAddress());
    //actor.send(message, actor.getAddress());
    //actor.send(message, actor.getAddress());
   std::thread t1([&actor,&message]()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (cnt == 0)
            {
                //exit(0);
            }
            //actor.send(message, actor.getAddress());
            //std::cout << cnt << std::endl;
            cnt = 0;
        }
    });
    framework.loop();

}