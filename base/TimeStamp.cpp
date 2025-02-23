#include "TimeStamp.h"
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>
#include <cassert>



string TimeStamp::toString() const
{
    char buf[32] = { 0 };
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

string TimeStamp::toFormattedString(bool showMicroseconds) const
{
    char buf[64] = { 0 };
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    auto year = tm_time.tm_year + 1900;
    auto month = tm_time.tm_mon + 1;
    auto day = tm_time.tm_mday;
    auto hour = tm_time.tm_hour;
    auto minute = tm_time.tm_min;
    auto second = tm_time.tm_sec;
    int microSeconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);

    if (showMicroseconds)
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
            year, month, day, hour, minute, second, microSeconds);
    else
        sprintf(buf, "%4d%02d%02d %02d:%02d:%02d",
            year, month, day, hour, minute, second);
    return buf;
}


TimeStamp TimeStamp::Now()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // 获取当前时间
    auto seconds = ts.tv_sec;
    auto nanoSeconds = ts.tv_nsec;
    return TimeStamp(static_cast<uint64_t>(seconds) * TimeStamp::kNanoSecondsPerSecond + nanoSeconds / 1000); // 转换为微秒
}

Second operator-(const TimeStamp &high, const TimeStamp &low)
{
    return static_cast<double>((high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch()) % TimeStamp::kMicroSecondsPerSecond);
}

TimeStamp operator+(const TimeStamp &t, Second seconds)
{
    if (!t.isvalid())
        return TimeStamp();
    return TimeStamp(t.microSecondsSinceEpoch() + seconds * TimeStamp::kMicroSecondsPerSecond);
}

timespec how_much_time_form_now(TimeStamp when)
{
    MicroSecond fut = when.microSecondsSinceEpoch(), now = TimeStamp::Now().microSecondsSinceEpoch();
    assert(now <= fut);

    MicroSecond microseconds = (fut - now) > 100 ? (fut - now) : 100;
    //MicroSecond microseconds = when.microSecondsSinceEpoch() - TimeStamp::Now().microSecondsSinceEpoch();
    struct timespec ts;
    Second seconds = microseconds / TimeStamp::kMicroSecondsPerSecond;
    NanoSecond nanoseconds = (microseconds % TimeStamp::kMicroSecondsPerSecond) * 1000;

    ts.tv_sec = static_cast<time_t>(seconds);
    ts.tv_nsec = static_cast<long>(nanoseconds);

    return ts;
}

