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
#include <unistd.h>
#include <memory>
#include "EventLoop.h"
#include "common.h"
using std::function;
using std::shared_ptr;
using std::weak_ptr;

class Channel {
public:
    DISALLOW_COPY_AND_MOV(Channel);
    Channel(int fd, EventLoop *loop) : fd_(fd), loop_(loop) { }
    ~Channel() { }

    void handle_event() const; // 处理事件
    void Tie(const shared_ptr<void> &obj); // 绑定

    void enable_read();  // 允许读
    void enable_write(); // 允许写
    void enable_ET();    // 以ET形式触发
    void disable_write() { }

    int fd() const { return fd_; }
    short listen_events() const { return listen_events_; }
    short ready_events() const { return ready_events_; }
    bool is_InEpoll() const { return in_epoll_; }

    void set_InEpoll(bool in = true) { in_epoll_ = in; }
    void set_ready_events(int ev) { ready_events_ = ev; }

    void set_read_callback(const function<void()>  &fn) { read_callback_ = fn; }
    void set_read_callback(function<void()> &&fn) { read_callback_ = std::move(fn); }
    
    void set_write_callback(const function<void()>  &fn) { write_callback_ = fn; }
    void set_write_callback(function<void()> &&fn) { write_callback_ = std::move(fn); }

private:
    int fd_ = -1;
    EventLoop *loop_ = nullptr;
    weak_ptr<void> tie_; // 用于绑定任何我们自定义的类型
    bool tied_ = false; // 是否绑定

    //监听的事件
    short listen_events_ = 0;
    short ready_events_ = 0;
    bool in_epoll_{ false };
    function<void()> read_callback_;
    function<void()> write_callback_;

    void handle_event_with_guard() const;
    void handle_event_without_guard() const;

    bool is_read_event() const { return ready_events_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP); }
    bool is_write_event() const { return ready_events_ & (EPOLLOUT); }
};

#endif