
#include <bits/types/FILE.h>
#include <cassert>
#include <memory>
#include <utility>

#include "CurrentThread.h"
#include "LoggerImpl.h"
#include "TimeStamp.h"

using std::make_unique;

__thread char t_time[FORMAT_TIME_BUFSIZE] = {0};
__thread time_t t_last_second = 0;

namespace
{

    struct Template
    {
        Template(const char *str, unsigned size) : str_(str), size_(size) {}
        const char *str_;
        const unsigned size_;
    };

    void defaultOutput(const char *msg, int len)
    {
        fwrite(msg, 1, len, stdout); // 默认写出到stdout
    }

    void defaultFlush()
    {
        fflush(stdout); // 默认flush到stdout
    }

    inline LogStream &operator<<(LogStream &stream, Template v)
    {
        stream.append(v.str_, v.size_);
        return stream;
    }
    OutputFunc global_output = defaultOutput;
    FlushFunc global_flush = defaultFlush;
    LogLevel global_level =
#ifdef _DEBUG
        LogLevel::DEBUG;
#else
        LogLevel::INFO;
#endif
    const char *GREEN = "\033[32m";
    const char *BLUE = "\033[34m";
    const char *ORANGE = "\033[33m";
    const char *RED = "\033[31m";
    const char *FATAL_RED = "\033[35m";

    const char *FINISH = "\033[0m";

    std::unordered_map<LogLevel, const char *> Level_map_ANSI{{LogLevel::DEBUG, GREEN},
                                                              {LogLevel::INFO, BLUE},
                                                              {LogLevel::WARN, ORANGE},
                                                              {LogLevel::ERROR, RED},
                                                              {LogLevel::FATAL, FATAL_RED}};
} // namespace

void Logger::setFlush(FlushFunc func) { global_flush = func; }
void Logger::setOutput(OutputFunc func) { global_output = func; }

LogLevel Logger::LEVEL() { return global_level; }
void Logger::SET_LEVEL(LogLevel level) { global_level = level; }

Logger::Logger(const char *file, int line, LogLevel level)
{
    impl_ = make_unique<LoggerImpl>(SourceFile(file), line, level);
}

Logger::~Logger()
{
    impl_->finish();
    const LogStream::Buffer &buf = impl_->stream().buffer();
    int size = buf.size();
    global_output(buf.data(), size);
    global_flush();

    if (impl_->level() == LogLevel::FATAL) {
        abort();
    }
}

LogStream &Logger::stream() { return impl_->stream(); }

LoggerImpl::LoggerImpl(SourceFile source, int line, LogLevel level) : level_(level), file_(source), line_(line)
{
    stream_ << Level_map_ANSI[level_];
    FormattedTime();
    CurrentThread::tid();
    stream_ << Template(CurrentThread::tidString(), CurrentThread::tidStringLength());
    stream_ << Template(format_level_string(), 6);
}

void LoggerImpl::FormattedTime()
{
    TimeStamp now = TimeStamp::Now();
    auto [Usec, micro_sec] = now.UnixSecondandMicroSecond();
    assert(t_last_second <= Usec);
    if (t_last_second < Usec) {
        now.toFormattedString(t_time);
        t_last_second = Usec;
    }
    stream_ << Template(t_time, strlen(t_time));
}

void LoggerImpl::finish() { stream_ << " - " << file_.data_ << ":" << line_ << FINISH << "\n"; }