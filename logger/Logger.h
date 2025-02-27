#ifndef LOGGER_H
#define LOGGER_H

#include "LogStream.h"
#include <bits/types/FILE.h>
#include <memory>

using std::unique_ptr;

using OutputFunc = void (*)(const char *, int); // 输出操作
using FlushFunc = void (*)(); //刷新操作

class LoggerImpl;
class LogStream;

enum class LogLevel
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
    Logger(const char *file, int line, LogLevel level);
    ~Logger();

    LogStream &stream();

    static LogLevel LEVEL();
    static void SET_LEVEL(LogLevel level);
    static void setOutput(OutputFunc); // 默认fwrite到stdout
    static void setFlush(FlushFunc);   // 默认fflush到stdout

  private:
    unique_ptr<LoggerImpl> impl_;
};

#define LOG_DEBUG                                                                                                      \
    if (Logger::LEVEL() <= LogLevel::DEBUG)                                                                            \
    Logger(__FILE__, __LINE__, LogLevel::DEBUG).stream()

#define LOG_INFO                                                                                                       \
    if (Logger::LEVEL() <= LogLevel::INFO)                                                                             \
    Logger(__FILE__, __LINE__, LogLevel::INFO).stream()

#define LOG_WARN  Logger(__FILE__, __LINE__, LogLevel::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, LogLevel::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, LogLevel::FATAL).stream()

#endif