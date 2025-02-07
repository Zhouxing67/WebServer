#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll;
using std::vector;

class EventLoop {
private:
    Epoll* ep_ = nullptr;
    bool quit_ = false;
    //核心嵌套类，实现事件循环

public:
    EventLoop();
    ~EventLoop();
    void quit() { quit_ = true; }

    //监听channel
    void channel_ctl(Channel *chl);
    void loop();
};
#endif