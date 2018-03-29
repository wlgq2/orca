#include     <ratio>
#include     <chrono>

#include "Log.h"

using namespace orca::log;

Log* Log::Instance()
{
    static Log handle;
    return &handle;
}


void Log::info(const std::string& message)
{

}

void Log::error(const std::string& message)
{

}

void Log::warn(const std::string& message)
{

}

void Log::debug(const std::string& message)
{

}

Log::Log()
{
    uv::Log::Instance()->registerInterface(this);
}
