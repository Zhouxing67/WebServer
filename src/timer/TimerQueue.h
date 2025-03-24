
#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include <unistd.h>
#include <sys/timerfd.h>

#include <set>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include "common.h"
#include "TimeStamp.h"

using std::unique_ptr;
using std::set;
using std::vector;

class Timer;
class EventLoop;
class Channel;

class TimerQueue
{
public:
    DISALLOW_COPY_AND_MOV(TimerQueue);
    TimerQueue(EventLoop *loop);
    ~TimerQueue();

    void add_Timer(TimeStamp timestamp, std::function<void()> const &cb, double interval); // 添加一个定时任务

private:
    typedef std::pair<TimeStamp, Timer *> Entry;
    int timerfd_;

    EventLoop *loop_;
    unique_ptr<Channel> channel_;

    set<Entry> timers_;                                   // 定时任务集合
    vector<Entry> active_timers_;                         // 激活的定时任务

    void handle_read();                                   //timerfd可读时，调用
    void reset_Timers();                                  //将具有重复属性的定时器重新加入队列
    bool insert_Timer_to_timerfd(Timer *timer);               //将定时任务加入队列

};
#endif