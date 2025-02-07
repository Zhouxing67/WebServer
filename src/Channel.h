#ifndef CHANNEL_H
#define CHANNEL_H
/*
Channel 类是一个对文件描述符 (file descriptor, FD) 的封装
在单Reactor模型中：一个fd通常只会对应一个Channel
epoll_event中的epoll_data是一个union，包含void*类型的ptr，通过ptr可以绑定任何我们自定义的类型
fd与其对应的channel通过epoll_ctl注册到内核中的epoll实例里，其中channel被绑定到ptr上；具体由EventLoop::handleChannel实现
 */
#include <functional>
#include <cstdint>
#include <sys/epoll.h>
#include "EventLoop.h"
using std::function;

class Channel {
private:
    friend class EventLoop;
    EventLoop *loop_ = nullptr;  // 所属的 EventLoop
    int fd_ = -1;                // 监听的 FD
    uint32_t events_ = -1;       // 关注的事件
    uint32_t revents_ = -1;      // 已触发的事件
    bool is_ctl_ = false;        // 是否已注册到 epoll
    function<void()> callback_;  // 事件处理函数

    void set_revents(uint32_t revents) { revents_ = revents; }
public:
    Channel();
    //个人认为初始化Channel时就应该设置事件处理函数
    Channel(EventLoop* loop, int fd, function<void()> func = []{});
    ~Channel();
    //设置关注事件（默认为ET触发模模式的可读事件），并通知epoll实例进行对自身的注册或更改
    void channel_ctl(unsigned int events = EPOLLIN | EPOLLET);
    void set_callback(std::function<void()> cb) { callback_ = cb; }

    int fd() { return fd_; }
    uint32_t revents() { return revents_; }
    uint32_t events() { return events_; }
    bool is_ctl()
    {
        bool res = is_ctl_;
        is_ctl_ = true;
        return res;
    }

    //当感兴趣的事件被触发时，调用此函数处理该事件
    void handle_event();

};
#endif