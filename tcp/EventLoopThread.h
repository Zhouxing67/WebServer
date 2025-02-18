#ifndef EVENT_LOOP_THREAD_H
#define EVENT_LOOP_THREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "EventLoop.h"
#include "common.h"

using std::thread;
using std::mutex;
using std::condition_variable;
using std::function;
using std::lock_guard;
using std::unique_lock;

class EventLoopThread {
public:
    DISALLOW_COPY_AND_MOV(EventLoopThread);
    EventLoopThread() = default;
    ~EventLoopThread();
    EventLoop *startLoop();

private:
    EventLoop *loop_ = nullptr;
    thread thread_;
    mutex mut_;
    condition_variable cv_;

    //构造EventLoopd对象
    void threadFunc();
};

#endif