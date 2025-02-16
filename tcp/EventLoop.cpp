#include "Channel.h"
#include "Epoller.h"
#include "EventLoop.h"
#include "Util.h"
#include <string.h> 
#include <iostream>
#include <mutex>

using std::mutex;
using std::lock_guard;

EventLoop::EventLoop()
{
    poller_ = std::make_unique<Epoller>();
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
}

void EventLoop::run_one_func(function<void()> fn)
{
    if (is_in_loop_thread())
        fn();
    else
        add_one_func(std::move(fn));
}

void EventLoop::do_todolist()
{
    vector<function<void()>> list;
    {
        lock_guard<mutex> lock(mut_);
        list.swap(todolist_);
    }
    for (auto &fn : list)
        fn();
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
