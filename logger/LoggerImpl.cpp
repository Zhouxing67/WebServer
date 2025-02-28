
#include <bits/types/FILE.h>
#include <cassert>
#include <cstdio>
#include <memory>
#include <utility>

#include "AsyncLogger.h"
#include "CurrentThread.h"
#include "Logger.h"
#include "LoggerImpl.h"
#include "TimeStamp.h"

using std::make_unique;

__thread char t_time[FORMAT_TIME_BUFSIZE] = {0};
__thread time_t t_last_second = 0;

namespace
{
    std::unique_ptr<AsyncLogger> ansyc_logging = make_unique<AsyncLogger>();

    [[maybe_unused]] void AnsycOutput(const char *msg, int len)
    {
        ansyc_logging->start();
        ansyc_logging->output(msg, len);
    }
    [[maybe_unused]] void defaultOutput(const char *msg, int len)
    {
        fwrite(msg, 1, len, stdout); // 默认写出到stdout
    }

    OutputFunc global_output =
#ifdef _ANSYC_LOG
        AnsycOutput;
#else
        defaultOutput;
#endif

    LOG_LEVEL global_level =
#ifdef _DEBUG
        LOG_LEVEL::DEBUG;
#else
        LogLevel::INFO;
#endif

    const char *GREEN = "\033[32m";
    const char *BLUE = "\033[34m";
    const char *ORANGE = "\033[33m";
    const char *RED = "\033[31m";
    const char *FATAL_RED = "\033[35m";
    const char *FINISH = "\033[0m";

    std::unordered_map<LOG_LEVEL, const char *> Level_map_ANSI{{LOG_LEVEL::DEBUG, GREEN},
                                                               {LOG_LEVEL::INFO, BLUE},
                                                               {LOG_LEVEL::WARN, ORANGE},
                                                               {LOG_LEVEL::ERROR, RED},
                                                               {LOG_LEVEL::FATAL, FATAL_RED}};
} // namespace

LOG_LEVEL Logger::LEVEL() { return global_level; }
void Logger::SET_LEVEL(LOG_LEVEL level) { global_level = level; }
void Logger::Async_Log() { global_output = AnsycOutput; }
void Logger::Sync_Log() { global_output = defaultOutput; }
void Logger::setOutput(OutputFunc func) { global_output = func; }

Logger::Logger(const char *file, int line, LOG_LEVEL level)
{
    impl_ = make_unique<LoggerImpl>(SourceFile(file), line, level);
}

Logger::~Logger()
{
    impl_->finish();
    const LogStream::Buffer &buf = impl_->stream().buffer();
    int size = buf.size();
    global_output(buf.data(), size);
    fflush(stdout);
    
    if (impl_->level() == LOG_LEVEL::FATAL) {
        abort();
    }
}
LogStream &Logger::stream() { return impl_->stream(); }

LoggerImpl::LoggerImpl(SourceFile source, int line, LOG_LEVEL level) : level_(level), file_(source), line_(line)
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