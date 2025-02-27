#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#include "Logger.h"
#include "common.h"
#include <unordered_map>



struct SourceFile
{
    SourceFile(const char *data) : data_(data), size_(static_cast<int>(strlen(data_)))
    { //使用strrchr消除文件名的目录部分
        const char *forward_slash = strrchr(data, '/');
        if (forward_slash) {
            data_ = forward_slash + 1;
            size_ -= static_cast<int>((data_ - data));
        }
    }
    const char *data_;
    int size_;
};

class LoggerImpl
{
  public:
    DISALLOW_COPY_AND_MOV(LoggerImpl)
    LoggerImpl(SourceFile file, int line, LogLevel level);

    const char *format_level_string() const; // 获取LogLevel的字符串
    void FormattedTime();
    // 完成格式化，并补充输出源码文件和源码位置
    void finish();

    LogLevel level() const { return level_; }
    LogStream &stream() { return stream_; }

  private:
    const LogLevel level_;
    SourceFile file_;
    int line_;
    LogStream stream_;
};

inline const char *LoggerImpl::format_level_string() const
{
    switch (level_) {
    case LogLevel::DEBUG:
        return "DEBUG ";
    case LogLevel::INFO:
        return "INFO  ";
    case LogLevel::WARN:
        return "WARN  ";
    case LogLevel::ERROR:
        return "ERROR ";
    case LogLevel::FATAL:
        return "FATAL ";
    }
    return nullptr;
}

#endif