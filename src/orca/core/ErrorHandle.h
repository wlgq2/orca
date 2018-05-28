#ifndef   ORCA_CORE_ERROR_HANDLE
#define   ORCA_CORE_ERROR_HANDLE

#include  <functional>
#include  <string>
#include  <iostream>

namespace orca
{

namespace core
{

class ErrorHandle
{
public:
    enum ErrorId
    {

    };
    using ErrorHandleFunction = std::function<void(int, std::string)>;

public:
    ErrorHandle* Instance()
    {
        static ErrorHandle errorHandle;
        return &errorHandle;
    }

    void bind(ErrorHandleFunction func)
    {
        handle_ = func;
    }
    void error(ErrorId id, std::string info)
    {
        if (handle_)
            handle_(id, info);
        else
            std::cerr << id<<":" << info;
    }
private:
    ErrorHandleFunction handle_;

    ErrorHandle() :handle_(nullptr){}

};

}
}
#endif // ! ORCA_CORE_ERROR_HANDLE

