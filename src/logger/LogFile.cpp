#include "LogFile.h"
#include "TimeStamp.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
LogFile::LogFile()
{
    std::string file_path = "log/LogFile_" + TimeStamp::Now().TimeStamp::toFormattedString();
    file_path.pop_back();
    file_path += ".log";
    fp_ = ::fopen(file_path.data(), "a+");
    if (fp_ == nullptr) {
        throw std::runtime_error("can not open log file\n");
        exit(1);
    }
}

LogFile::~LogFile()
{
    Flush();
    if (!fp_) {
        fclose(fp_);
    }
}

void LogFile::Write(const char *data, int len)
{

    fwrite(data, 1, len, fp_);
    written_bytes_ += len;

    time_t now = ::time(nullptr);

    // 更新当前状态
    if (len != 0) {
        lastwrite_ = now;
    }
    // 判断是否需要Flush
    if (lastwrite_ - lastflush_ > FlushInterval) {
        Flush();
        lastflush_ = now;
    }
}

int64_t LogFile::writtenbytes() const { return written_bytes_; }

void LogFile::Flush() { fflush(fp_); }