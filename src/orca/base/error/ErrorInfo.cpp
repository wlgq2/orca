#include "ErrorInfo.h"

using namespace orca::base;

orca::base::ErrorInfo::ErrorInfo(ErrorId id, std::string info)
    :id_(id),
    info_(info)
{
}

ErrorInfo::ErrorId orca::base::ErrorInfo::getErrorId()
{
    return id_;
}

std::string& orca::base::ErrorInfo::getErrorInfo()
{
    return info_;
}
