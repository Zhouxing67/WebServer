#include <algorithm>
#include "TimerQueue.h"
#include "Timerfd.h"
#include "Timer.h"
#include "EventLoop.h"
#include "Channel.h"

using std::make_unique;
TimerQueue::TimerQueue(EventLoop *loop) : loop_(loop)
{
    timerfd_ = create_timerfd();
    channel_ = make_unique<Channel>(timerfd_, loop_);
    channel_->set_read_callback([this]
        {
            handle_read();
        });
    channel_->enable_read();
}

TimerQueue::~TimerQueue()
{
    loop_->delete_channel(channel_.get());
    loop_ = nullptr;
    close(timerfd_);

    for (auto &[_, timer] : timers_)
        delete timer;
}

void TimerQueue::add_Timer(TimeStamp timestamp, std::function<void()> const &cb, double interval)
{
    Timer *timer = new Timer(timestamp, cb, interval);
    if (insert_Timer_to_timerfd(timer))
        reset_timerfd(timestamp,timerfd_);
}

void TimerQueue::handle_read()
{
    read_timerfd(TimeStamp::Now(), timerfd_);
    auto lastest = timers_.lower_bound({ TimeStamp::Now(), reinterpret_cast<Timer *>(UINTPTR_MAX) });
    active_timers_.clear();
    active_timers_.insert(active_timers_.end(), timers_.begin(), lastest);

    timers_.erase(timers_.begin(), lastest);

    for (auto &[_, timer] : active_timers_) {
        timer->run();
    }
    reset_Timers();
    if(!timers_.empty())
        reset_timerfd(timers_.begin()->first, timerfd_);
}

void TimerQueue::reset_Timers()
{
    for (auto &[_, timer] : active_timers_) {
        if (timer->is_repeat())
        {
            timer->restart();
            insert_Timer_to_timerfd(timer);
        }
        
        else {
            delete timer;
            timer = nullptr;
        }
    }
}

bool TimerQueue::insert_Timer_to_timerfd(Timer *timer)
{
    bool reset_timerfd_instant = false;
    if (timers_.empty() || timer->expiration() < timers_.begin()->first)
        reset_timerfd_instant = true;
    timers_.insert({ timer->expiration() , timer });
    return reset_timerfd_instant;
}
