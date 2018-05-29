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
        UVWriteFail = -2048,
        UVConnectFail,
        UVDisconnectFromServer,

        UndefinedError  = -1024,
        NoFindActorName,
        NoFindActorAddr,
        ReDefineActorName,
    };
    using ErrorHandleFunction = std::function<void(ErrorId, std::string&)>;

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
    void error(ErrorId id, std::string& info)
    {
        if (handle_)
            handle_(id, info);
        else
            std::cerr << "error id "<<id << ":" << info << std::endl;
    }
    void error(ErrorId id, std::string&& info)
    {
        error(id, info);
    }
private:
    ErrorHandleFunction handle_;

    ErrorHandle() :handle_(nullptr){}

};

}
}
#endif // ! ORCA_CORE_ERROR_HANDLE

