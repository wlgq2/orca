#include <iostream>
#include <memory>
#include <vector>

#include <orca/orca.h>
#include <future>
#include <orca/base/libuv_cpp11/uv/uv11.h>

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
    static const unsigned long Size = 512;
    char message[Size];
};

REGISTER_MESSAGE_TYPE(MyMessage);

int main(int argc, char** args)
{
    orca::Framework framework;

    orca::MessagePack message;
    message.create("a message from actor1");
    orca::Actor actor1(&framework);
    orca::Actor actor2(&framework);
    actor1.registerHandler([&actor2](const orca::MessagePack& pack, orca::Address& from)
    {
        std::cout <<"actor1 recieve£º"<< pack.get()->size() << ":" << (char*)(pack.get()->enter()) << std::endl;
    });
    actor2.registerHandler([&actor2](const orca::MessagePack& pack,orca::Address& from)
    {
        std::cout << "actor2 recieve£º" << pack.get()->size()<<":" << (char*)(pack.get()->enter()) << std::endl;
        orca::MessagePack message;
        message.create("a message form acotr2");
        actor2.send(message, from);
    });
    actor1.send(message, actor2.getAddress());

    framework.loop();

}