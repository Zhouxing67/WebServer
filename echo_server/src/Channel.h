#ifndef CHANNEL_H
#define CHANNEL_H
/*
封装文件描述符，以支持特定应用
 */
#include "EventLoop.h"
#include <functional>

using std::function;

namespace
{
    void noOp() { }
}

//封装文件描述符，支持业务逻辑细分
class Channel {
private:
    

    EventLoop* loop_ = nullptr;
    int fd_ = -1;
    uint32_t events_ = -1;
    uint32_t revents_ = -1;
    bool is_ctl_ = false; 
    function<void()> callback_;
public:
    Channel();
    Channel(EventLoop* loop, int fd, function<void()> = noOp);
    ~Channel();

    int getFD() { return fd_; }
    //设置触发条件
    void channel_ctl(unsigned int events = EPOLLIN | EPOLLET);
    uint32_t events() { return events_; }
    uint32_t revents() { return revents_; }
    bool is_ctl()
    {
        bool res = is_ctl_;
        is_ctl_ = true;
        return res;
    }

    //当感兴趣的事件被触发时，调用此函数处理该事件
    void handleEvent();
    void setcallback(std::function<void()> cb) { callback_ = cb; }

    //void events(uint32_t events) { events_ = events; }
    void set_revents(uint32_t revents) { revents_ = revents; }

};
#endif