#ifndef  ASYNC_LOG_H
#define  ASYNC_LOG_H


#include     <string>
#include     <spdlog/spdlog.h>
#include     <uv/uv11.h>

class AsyncLog :public uv::LogInterface
{
public:
public:
    AsyncLog()
        : async_(nullptr),
        console_(spdlog::stdout_color_mt("console"))
    {

    }

    void initLogFile(std::string name, size_t size)
    {
        spdlog::set_async_mode(size);
        async_ = spdlog::daily_logger_st(name, "logs/" + name);
        spdlog::set_error_handler([this](const std::string& msg)
        {
            error(msg);
        });
    }

    void info(const std::string& message) override
    {
        console_->info(message);
        writeLog(spdlog::level::info, message);
    }

    void error(const std::string& message) override
    {
        console_->error(message);
        writeLog(spdlog::level::err, message);
    }

    void warn(const std::string& message) override
    {
        console_->warn(message);
        writeLog(spdlog::level::warn, message);
    }

    void debug(const std::string& message) override
    {
        console_->debug(message);
        writeLog(spdlog::level::debug, message);
    }
private:

    void writeLog(spdlog::level::level_enum level, const std::string& message)
    {
        if (async_)
            async_->log(level, message);
    }

    std::shared_ptr<spdlog::logger> async_;
    std::shared_ptr<spdlog::logger> console_;
};
#endif // ! LOG_OUT_H

