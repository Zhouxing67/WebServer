#include "EventLoopThread.h"

EventLoopThread::~EventLoopThread()
{
    if (thread_.joinable())
        thread_.join();
}

//启动子线程，返回EventLoop对象指针
EventLoop *EventLoopThread::startLoop()
{
    thread_ = thread(&EventLoopThread::threadFunc, this);
    EventLoop *loop = nullptr;
    {
        unique_lock<mutex> lock(mut_);
        cv_.wait(lock, [this] { return loop_ != nullptr; });
        loop = loop_;
    }
    return loop;
}

//构造EventLoop对象
void EventLoopThread::threadFunc()
{

    EventLoop loop;
    {
        lock_guard<mutex> lock(mut_);
        loop_ = &loop;
        cv_.notify_one();
    }
    loop_->loop();
    {
        lock_guard<mutex> lock(mut_);
        loop_ = nullptr;
    }
}