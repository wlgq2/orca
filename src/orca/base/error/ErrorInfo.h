#ifndef   ORCA_BASE_ERROR_INFO_H
#define   ORCA_BASE_ERROR_INFO_H

#include  <string>

namespace orca
{
namespace base
{
class ErrorInfo
{
public:
    enum ErrorId
    {
        UVWriteFail = -2048,
        UVConnectFail,
        UVDisconnectFromServer,
        UVSigPipe,

        UndefinedError = -1024,
        NoFindActorName,
        ActorNameTooLong,
        NoFindActorAddr,
        ReDefineActorName,
        MessagePackNull,
        PackMessageError,
        NoFindRemoteFramework,

        RepeatedRemoteFrameworkID,
    };
    ErrorInfo(ErrorId id,std::string info);

    ErrorId getErrorId();
    std::string& getErrorInfo();

private:
    ErrorId id_;
    std::string info_;
};
}
}
#endif // !  ORCA_BASE_ERROR_INFO_H

