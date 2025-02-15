#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/epoll.h>
#include <vector>
#include <memory>
#include <mutex>
#include "common.h"

using std::function;
using std::mutex;
using std::vector;
using std::unique_ptr;

class Channel;
class Epoller;
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
    //void add_one_func(const function<void()> &fn) {}

private:
    unique_ptr<Epoller> poller_;
    vector<function<void()>> todolist_;
    mutex mut_;

    void do_todolist();
};
#endif