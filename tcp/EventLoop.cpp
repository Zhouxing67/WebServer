#include "Channel.h"
#include "Epoller.h"
#include "Util.h"
#include "TimerQueue.h"
#include "EventLoop.h"
#include <string.h> 
#include <iostream>
#include <mutex>
#include <sys/eventfd.h>
#include <cassert>

using std::mutex;
using std::lock_guard;
using std::make_unique;

EventLoop::EventLoop()
{
    poller_ = make_unique<Epoller>();

    //创建一个eventfd，用于唤醒loop
    wakeup_fd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    wakeup_channel_ = make_unique<Channel>(wakeup_fd_, this);
    wakeup_channel_->set_read_callback([this] {
        uint64_t one;
        int read_size = ::read(wakeup_fd_, &one, sizeof(one));
        (void)(read_size);
        assert(read_size == sizeof(one));
        });

    wakeup_channel_->enable_read();
    timerQue_ = make_unique<TimerQueue>(this);
}

EventLoop::~EventLoop() { }

void EventLoop::update_channel(Channel *chl) const
{
    poller_->update_channel(chl);
}

void EventLoop::delete_channel(Channel *chl) const
{
    poller_->delete_channel(chl);
}

void EventLoop::add_one_func(function<void()> fn)
{
    lock_guard<mutex> lock(mut_);
    todolist_.emplace_back(std::move(fn));
    // 如果调用当前函数的并不是当前EventLoop对应的的线程，将其唤醒。主要用于关闭TcpConnection
    // 关闭连接是由对应`TcpConnection`所发起的
    // 关闭连接的操作应该由main_reactor所进行(为了释放ConnectionMap的所持有的TcpConnection)
    if (!is_in_loop_thread() || is_doing_todolist_)
        wakeup();
}

void EventLoop::run_one_func(function<void()> fn)
{
    if (is_in_loop_thread())
        fn();
    else
        add_one_func(std::move(fn));
}

void EventLoop::run_at(TimeStamp timestamp, std::function<void()> cb)
{
    timerQue_->add_Timer(timestamp, cb, -1.0);
}

void EventLoop::run_after(Second wait_time, std::function<void()> cb)
{
    timerQue_->add_Timer(TimeStamp::Now() + wait_time, cb, -1.0);
}

void EventLoop::run_every(Second interval, std::function<void()> cb)
{
    timerQue_->add_Timer(TimeStamp::Now() + interval, cb, interval);
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t write_size = ::write(wakeup_fd_, &one, sizeof(one));
    (void)(write_size);
    assert(write_size == sizeof(one));
    return;
}

void EventLoop::do_todolist()
{
    is_doing_todolist_ = true;
    vector<function<void()>> list;
    {
        lock_guard<mutex> lock(mut_);
        list.swap(todolist_);
    }
    for (auto &fn : list)
        fn();
    is_doing_todolist_ = false;
}

//每次事件循环，获取被触发的channel列表，让每一个活动channel处理自己的事件
void EventLoop::loop()
{
    tid_ = CurrentThread::tid();
    while (true) {
        for (auto channel : poller_->poll())
            channel->handle_event();
        do_todolist();
    }
}
