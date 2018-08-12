#include <iostream>
#include "MessageType.h"

#define   USE_END_POINT_1   1

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
        std::cout << (char*)(pack.enter()) << std::endl;
#if     !USE_END_POINT_1
        //remote message return.远程消息返回。
        send(pack, from);
#endif
    }
};

int main(int argc, char** args)
{
    //complie endpoint 1 编译端点1.
#if    USE_END_POINT_1
    //framework configs.
    orca::FrameworkConfig config;
    config.id = 1024;
    config.threadCount = 1;
    config.endPointAddress = std::make_shared<orca::EndPointAddress>("0.0.0.0",10001, orca::EndPointAddress::Ipv4);

    orca::Framework framework(config);
    //append remote framework.
    framework.appendRemoteEndPoint("127.0.0.1", 10002);

    ActorTest actor(&framework);
    //message pack.
    char data[] = "a message from remote actor";
    orca::MessagePack message;
    message.create(data,sizeof(data));

    //actor->actor2(remote) send message.
    actor.send(message,"actor2",1025);
    framework.loop();
#else  
    //complie endpoint 2 编译端点2.

    //framework configs.
    orca::FrameworkConfig config;
    config.id = 1025;
    config.threadCount = 1;
    config.endPointAddress = std::make_shared<orca::EndPointAddress>("0.0.0.0", 10002, orca::EndPointAddress::Ipv4);

    orca::Framework framework(config);
    //append remote framework.
    framework.appendRemoteEndPoint("127.0.0.1", 10001);

    ActorTest actor(&framework,"actor2");
    framework.loop();
#endif
}