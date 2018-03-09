#ifndef     ORCA_LOG_H
#define     ORCA_LOG_H

#include    "AsyncLog.h"

namespace orca
{
namespace log
{

void setLevel(Level level);

void init(std::string file, size_t filesize, size_t fileCnt, size_t cache, int cycleMs = 1);

void info(const std::string& message);
void info(const std::string&& message);

void error(const std::string& message);
void error(const std::string&& message);

void warn(const std::string& message);
void warn(const std::string&& message);

void debug(const std::string& message);
void debug(const std::string&& message);

}
}
#endif // ! ORCA_LOG_H

