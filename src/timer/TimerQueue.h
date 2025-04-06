
#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include <sys/timerfd.h>
#include <unistd.h>

#include "TimeStamp.h"
#include "Timer.h"
#include "common.h"
#include <functional>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using std::set;
using std::unique_ptr;
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
    int timerfd_;

    EventLoop *loop_;
    unique_ptr<Channel> channel_;

    struct cmpTimer
    {
        bool operator()(Timer *l, Timer *r) const { return l->expiration() < r->expiration(); }
    };

    set<Timer *, cmpTimer> timers_;      // 定时任务集合
    vector<Timer *> active_timers_; // 激活的定时任务

    void handle_read();                         // timerfd可读时，调用
    void reset_Timers();                        //将具有重复属性的定时器重新加入队列
    bool insert_Timer_to_timerfd(Timer *timer); //将定时任务加入队列
};
#endif