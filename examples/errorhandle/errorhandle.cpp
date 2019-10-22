#include <iostream>
#include <orca/orca.h>

                                                    
REGISTER_MESSAGE_TYPE(std::string);

void errorHandle(orca::base::ErrorInfo info)
{
    std::cout << "error id : " << info.getErrorId() << std::endl;
    std::cout << "error message: " << info.getErrorInfo() << std::endl;
}

int main(int argc, char** args)
{
    //actor framework.
    orca::Framework framework;

    framework.RegisterErrorHandle(std::bind(&errorHandle,std::placeholders::_1));

    orca::base::ErrorHandle::Instance()->error(orca::base::ErrorInfo::UndefinedError,"undefine error");
    framework.loop();

}