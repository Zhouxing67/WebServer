#ifndef TIME_STAMP_H
#define TIME_STAMP_H

#include <ctime>
#include <string>
#include <utility>

using std::string;


typedef double Second; 
typedef uint64_t MicroSecond;
typedef uint64_t NanoSecond;
typedef std::pair<time_t, MicroSecond> UnixSecond_And_MicroSecond;

//Time stamp in UTC, in microseconds resolution.
class TimeStamp
{
public:
    static const int kMicroSecondsPerSecond = 1000 * 1000;
    static const int kNanoSecondsPerSecond = 1000 * 1000 * 1000;


    TimeStamp() = default; //默认构造函数,isvalid() == false
    explicit TimeStamp(MicroSecond microSecondsSinceEpoch) : microSecondsSinceEpoch_(microSecondsSinceEpoch) { }
    TimeStamp(const TimeStamp &rhs) = default;
    TimeStamp &operator=(const TimeStamp &rhs) = default;
    ~TimeStamp() = default;

    bool operator< (const TimeStamp &rhs) const { return microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_; }
    bool operator== (const TimeStamp &rhs) const { return microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_; }

    string toString() const;
    string toFormattedString(bool showMicroseconds = true) const;

    MicroSecond microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
    time_t UnixSecondsSinceEpoch() const { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }
    Second secondsSinceEpoch() const { return static_cast<Second>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }
    UnixSecond_And_MicroSecond UnixSecondandMicroSecond()
    {
        return { UnixSecondsSinceEpoch(), microSecondsSinceEpoch() % kMicroSecondsPerSecond };
    }

    bool isvalid() const { return microSecondsSinceEpoch_ > 0; }

    //now
    static TimeStamp Now();
    //返回一个无效的TimeStamp
    static TimeStamp Invalid() { return TimeStamp(); }
    //使用unix时间戳构造TimeStamp
    static TimeStamp fromUnixTime(time_t t) { return TimeStamp(static_cast<MicroSecond>(t * kMicroSecondsPerSecond)); };
    static TimeStamp fromUnixTime(time_t t, int microseconds) { return TimeStamp(static_cast<MicroSecond>(t * kMicroSecondsPerSecond + microseconds)); }

private:
    //从epoch_到现在的微秒数
    MicroSecond microSecondsSinceEpoch_ = 0;
};


Second operator-(const TimeStamp &high, const TimeStamp &low);
TimeStamp operator+(const TimeStamp &when, Second seconds);

timespec how_much_time_form_now(TimeStamp when);
#endif