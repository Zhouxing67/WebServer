#ifndef TIMER_H
#define TIMER_H


#include <functional>
#include "common.h"
#include "TimeStamp.h"

using std::function;

//timer event.定时任务
class Timer
{
public:
    DISALLOW_COPY_AND_MOV(Timer);
    Timer(TimeStamp when, function<void()> cb, Second interval) :
        expiration_(when), callback_(std::move(cb)), interval_(interval), repeat_(interval > 0.0)
    { }

    ~Timer() = default;
    TimeStamp expiration() const { return expiration_; }
    double interval() const { return interval_; }
    bool is_repeat() const { return repeat_; }
    void run() const { callback_(); }
    //when时刻重启；执行时间为when+interval后;
    void restart(TimeStamp when);
    //现在重启：执行时间为interval后
    void restart();


private:
    TimeStamp expiration_; // 时间戳成员记录当前定时任务的执行时间
    function<void()> callback_; //
    //重复任务
    const Second interval_; //执行周期
    const bool repeat_;
};
#endif