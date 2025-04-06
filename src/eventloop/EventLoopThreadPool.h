#ifndef EVENT_LOOP_THREAD_POOL_H
#define EVENT_LOOP_THREAD_POOL_H

#include <vector>
#include <memory>
#include "EventLoopThread.h"

using std::vector;
using std::unique_ptr;

class EventLoopThreadPool
{
public:
    DISALLOW_COPY_AND_MOV(EventLoopThreadPool);
    EventLoopThreadPool(EventLoop *loop);
    ~EventLoopThreadPool();

    void start();
    EventLoop *get_next_loop();
    unsigned int get_concurrency() const { return concurrency_; }
    
private:
    EventLoop *main_reactor_loop_ = nullptr;
    vector<unique_ptr<EventLoopThread>> threads_;
    vector<EventLoop *> sub_reactors_;

    const unsigned int concurrency_;
    bool started_ = false;
    unsigned int get_next()
    {
        static unsigned int next = 0;
        if(next >= concurrency_)
            next = 0;
        return next++;
    }

};
#endif