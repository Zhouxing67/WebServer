#include "EventLoopThreadPool.h"
#include <cassert>

using std::make_unique;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop) :
    main_reactor_loop_(loop), concurrency_(thread::hardware_concurrency())
{ }

EventLoopThreadPool::~EventLoopThreadPool()
{
    main_reactor_loop_ = nullptr;
}

void EventLoopThreadPool::start()
{
    for (unsigned int i = 0; i < concurrency_; ++i)
    {
        unique_ptr<EventLoopThread> thread = make_unique<EventLoopThread>();
        threads_.push_back(std::move(thread));
        sub_reactors_.push_back(threads_[i]->startLoop());
    }
    started_ = true;
}

EventLoop *EventLoopThreadPool::get_next_loop()
{
    assert(started_);
    static unsigned int next = get_next();
    EventLoop *sub_reactor_loop_ = sub_reactors_[next];
    return sub_reactor_loop_;
}
