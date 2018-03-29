#ifndef  ORCA_ASYNC_LOG_H
#define  ORCA_ASYNC_LOG_H


#include     <string>
#include     "../libuv_cpp11/uv/uv11.h"

namespace orca
{
namespace log
{

enum Level
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Critical = 5,
    Off = 6
};

class Log :public uv::LogInterface
{
public:
    static Log* Instance();

    void info(const std::string& message) override;

    void error(const std::string& message) override;

    void warn(const std::string& message) override;

    void debug(const std::string& message) override;
private:
    Log();
};

}
}
#endif
