#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <orca/orca.h>

class MyMessage
{
public:
    MyMessage(char* str, int size)
    {
        if (size > Size)
            size = Size;
        std::copy(str, str+size,message);
    }
    ~MyMessage()
    {
    }

    const unsigned long size()
    {
        return Size;
    }
    const char* enter()
    {
        return message;
    }

private:
    static const int Size = 1024;
    char message[Size];
};

//registet messge type.注册自定义消息类型。
REGISTER_MESSAGE_TYPE(MyMessage);

#endif