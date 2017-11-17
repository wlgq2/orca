#include <iostream>

#include <orca/MessageBox.h>

using namespace  std;

class MyMessage
{
public:
    MyMessage(const char* str)
    {
        memset(message, 0x00, Size);
        memcpy(message, str, strlen(str));
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



int main(int argc, char** args)
{
    orca::MessageBox<string> messageStr(new string("string message type"));
    cout << (char*)messageStr.enter() << " :" << messageStr.size()<< endl;


    orca::MessageBox<MyMessage> message;
    message.create("my message type");

    cout << (char*)message.enter() <<" :"<< message.size() << endl;


    system("pause");
}