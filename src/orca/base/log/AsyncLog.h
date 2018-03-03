#ifndef  ORCA_ASYNC_LOG_H
#define  ORCA_ASYNC_LOG_H


#include     <string>
#include     <spdlog/spdlog.h>
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

class AsyncLog :public uv::LogInterface
{
public:
    static AsyncLog* Instance();

    void setLevel(Level level);

    void initLogFile(std::string name, size_t size,int cycleMs);

    void info(const std::string& message) override;

    void error(const std::string& message) override;

    void warn(const std::string& message) override;

    void debug(const std::string& message) override;
private:
    AsyncLog();

    void writeLog(spdlog::level::level_enum level, const std::string& message);

    std::shared_ptr<spdlog::logger> async_;
    std::shared_ptr<spdlog::logger> console_;
};

}
}
#endif
