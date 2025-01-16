#ifndef CHANNEL_H
#define CHANNEL_H
#include "Epoll.h"

//封装文件描述符，支持业务逻辑细分
class Channel {
private:
    Epoll* ep_ = nullptr;
    int fd_ = -1;
    uint32_t events_ = -1;
    uint32_t revents_ = -1;
    bool is_ctl_ = false;
public:
    Channel();
    Channel(Epoll* ep, int fd);
    ~Channel();

    int getFD() { return fd_; }
    void channel_ctl(int events = EPOLLIN | EPOLLET);
    uint32_t events() { return events_; }
    uint32_t revents() { return revents_; }
    bool is_ctl() { return is_ctl_; }


    //void events(uint32_t events) { events_ = events; }
    void set_revents(uint32_t revents) { revents_ = revents; }

};
#endif