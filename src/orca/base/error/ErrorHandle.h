#ifndef   ORCA_BASE_ERROR_HANDLE
#define   ORCA_BASE_ERROR_HANDLE

#include  <functional>
#include  <string>
#include  <iostream>
#include  "ErrorInfo.h"


namespace orca
{

namespace base
{

class ErrorHandle
{
public:
    using ErrorHandleFunction = std::function<void(ErrorInfo)>;

public:
    static ErrorHandle* Instance()
    {
        static ErrorHandle errorHandle;
        return &errorHandle;
    }

    void bind(ErrorHandleFunction func)
    {
        handle_ = func;
    }
    void error(ErrorInfo info)
    {
        if (handle_)
            handle_(info);
        else
            std::cerr << "error id "<< info.getErrorId() << ":" << info.getErrorInfo() << std::endl;
    }
    void error(ErrorInfo::ErrorId id, std::string&& info)
    {
        ErrorInfo errorInfo(id, info);
        error(errorInfo);
    }
    void error(ErrorInfo::ErrorId id, std::string& info)
    {
        ErrorInfo errorInfo(id, info);
        error(errorInfo);
    }
private:
    ErrorHandleFunction handle_;

    ErrorHandle() :handle_(nullptr){}

};

}
}
#endif // ! ORCA_CORE_ERROR_HANDLE

