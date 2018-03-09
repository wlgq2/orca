#include     <ratio>
#include     <chrono>

#include "AsyncLog.h"

using namespace orca::log;

AsyncLog* AsyncLog::Instance()
{
    static AsyncLog handle;
    return &handle;
}

void AsyncLog::setLevel(Level level)
{
    spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
}

void AsyncLog::initLogFile(std::string name, size_t filesize, size_t fileCnt, size_t cache, int cycleMs)
{
    spdlog::set_async_mode(cache, spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::milliseconds(cycleMs));

    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    std::string filename(name);
    filename += std::to_string(tm.tm_year + 1900) + ".";
    filename += std::to_string(tm.tm_mon) + ".";
    filename += std::to_string(tm.tm_mday) + " ";
    filename += std::to_string(tm.tm_hour) + "-";
    filename += std::to_string(tm.tm_min) + "-";
    filename += std::to_string(tm.tm_sec) + " ";
    try
    {
        async_ = spdlog::rotating_logger_mt(name, "logs/" + filename, filesize, fileCnt);
    }
    catch (...)
    {
        async_ = nullptr;
        console_->error("fail to open 'logs' directory");
    }
    spdlog::set_error_handler([this](const std::string& msg)
    {
        error(msg);
    });
}

void AsyncLog::info(const std::string& message)
{
    console_->info(message);
    writeLog(spdlog::level::info, message);
}

void AsyncLog::error(const std::string& message)
{
    console_->error(message);
    writeLog(spdlog::level::err, message);
}

void AsyncLog::warn(const std::string& message)
{
    console_->warn(message);
    writeLog(spdlog::level::warn, message);
}

void AsyncLog::debug(const std::string& message)
{
    console_->debug(message);
    writeLog(spdlog::level::debug, message);
}

AsyncLog::AsyncLog()
    : async_(nullptr),
    console_(spdlog::stdout_color_mt("console"))
{
    uv::Log::Instance()->registerInterface(this);
}

void AsyncLog::writeLog(spdlog::level::level_enum level, const std::string& message)
{
    if (async_)
        async_->log(level, message);
}