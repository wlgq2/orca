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
        std::cout << "release" << std::endl;
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
    orca::MessagePack message;
    message.create("my message type");

    cout << (char*)message.enter() <<" :"<< message.size() << endl;

    orca::Framework framework;

    //orca::core::Actor<> a(&framework);

    orca::Actor send(&framework);
    orca::Actor recieve(&framework);
    recieve.registerHandler([](const orca::MessagePack& pack, const orca::Address& addr)
    {
        std::cout << pack.get()->size()<<":" << (char*)(pack.get()->enter()) << std::endl;
    });
    send.send(message, recieve.getAddress());

    framework.loop();

}