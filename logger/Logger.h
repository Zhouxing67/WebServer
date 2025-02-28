#ifndef LOGGER_H
#define LOGGER_H

#include "LogStream.h"
#include <bits/types/FILE.h>
#include <memory>

using std::unique_ptr;

using OutputFunc = void (*)(const char *, int); // 输出操作

class LoggerImpl;
class LogStream;

enum class LOG_LEVEL
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger
{
  public:
    Logger(const char *file, int line, LOG_LEVEL level);
    ~Logger();

    LogStream &stream();

    static LOG_LEVEL LEVEL();
    static void SET_LEVEL(LOG_LEVEL level);
    static void setOutput(OutputFunc); // 默认fwrite到stdout

    static void Async_Log();
    static void Sync_Log();
  private:
    unique_ptr<LoggerImpl> impl_;
};

#define LOG_DEBUG                                                                                                      \
    if (Logger::LEVEL() <= LOG_LEVEL::DEBUG)                                                                            \
    Logger(__FILE__, __LINE__, LOG_LEVEL::DEBUG).stream()

#define LOG_INFO                                                                                                       \
    if (Logger::LEVEL() <= LOG_LEVEL::INFO)                                                                             \
    Logger(__FILE__, __LINE__, LOG_LEVEL::INFO).stream()

#define LOG_WARN  Logger(__FILE__, __LINE__, LOG_LEVEL::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, LOG_LEVEL::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, LOG_LEVEL::FATAL).stream()

#endif