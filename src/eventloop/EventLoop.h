#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <functional>
#include <sys/epoll.h>
#include <vector>
#include <memory>
#include <mutex>  
#include "common.h"
#include "CurrentThread.h"
#include "TimeStamp.h"

using std::function;
using std::mutex;
using std::vector;
using std::unique_ptr;

class Channel;
class Epoller;
class TimerQueue;
class EventLoop
{
public:
    DISALLOW_COPY_AND_MOV(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(Channel *ch) const;
    void delete_channel(Channel *ch) const;
    void add_one_func(function<void()> fn);
    void run_one_func(function<void()> fn);
    //void add_one_func(const function<void()> &fn) {}

    void run_at(TimeStamp timestamp, std::function<void()> cb);
    void run_after(Second wait_time, std::function < void()> cb);
    void run_every(Second interval, std::function<void()> cb);

private:
    unique_ptr<Epoller> poller_;
    vector<function<void()>> todolist_;
    mutex mut_;

    //执行EventLoop::loop的线程id（所有的EventLoop都由主线程创建，但sub_reactor的EventLoop::loop的调用会被分配到子线程）
    const pid_t tid_;

    int wakeup_fd_;
    unique_ptr<Channel> wakeup_channel_;
    bool is_doing_todolist_ = false;

    unique_ptr<TimerQueue> timerQue_;

    void wakeup();//向wakeup_fd_写一个字节，唤醒loop
    // 判断调用该函数的是不是调用EventLoop::loop的线程。
    bool is_in_loop_thread() const { return tid_ == CurrentThread::tid(); }
    void do_todolist();
};
#endif