#include "log.h"

void orca::log::setLevel(Level level)
{
    AsyncLog::Instance()->setLevel(level);
}

void orca::log::init(std::string file, size_t filesize, size_t fileCnt, size_t cache, int cycleMs)
{
    AsyncLog::Instance()->initLogFile(file, filesize,fileCnt,cache, cycleMs);
}

void orca::log::info(const std::string& message)
{
    AsyncLog::Instance()->info(message);
}

void orca::log::info(const std::string&& message)
{
    AsyncLog::Instance()->info(message);
}

void orca::log::error(const std::string& message)
{
    AsyncLog::Instance()->error(message);
}

void orca::log::error(const std::string&& message)
{
    AsyncLog::Instance()->error(message);
}

void orca::log::warn(const std::string& message)
{
    AsyncLog::Instance()->warn(message);
}

void orca::log::warn(const std::string&& message)
{
    AsyncLog::Instance()->warn(message);
}

void orca::log::debug(const std::string& message)
{
    AsyncLog::Instance()->debug(message);
}

void orca::log::debug(const std::string&& message)
{
    AsyncLog::Instance()->debug(message);
}