#include "EventLoop.h"
#include "Util.h"
#include <string.h>
#include "Channel.h"

epoll_event EventLoop::Epoll::events[MAX_EVENTS];
EventLoop::EventLoop() : ep_(new Epoll())
{

}

EventLoop::~EventLoop()
{
    delete ep_;
    ep_ = nullptr;
}

void EventLoop::handleChannel(Channel* chl)
{
    ep_->handleChannel(chl);
}

void EventLoop::Epoll::handleChannel(Channel* chl)
{
    epoll_event ev;
    epoll_data_t data;
    data.ptr = chl;
    int fd = chl->getFD();
    set_ev(ev, data, chl->events());
    if (chl->is_ctl())
        ctl(EPOLL_CTL_MOD, fd, &ev);
    else {
        ctl(EPOLL_CTL_ADD, fd, &ev);
        chl->is_ctl();
    }
}

vector<Channel*> EventLoop::Epoll::poll(int timeout)
{
    bzero(events, sizeof(events));
    int nfds = epoll_wait(epfd_, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll_wait error");
    vector<Channel*> activeCahnnels(nfds, nullptr);
    for (int i = 0; i < nfds; i++)
        activeCahnnels[i] = static_cast<Channel*>(events[i].data.ptr);
    return activeCahnnels;
}

//每次事件循环，获取被触发的channel列表，让每一个活动channel处理自己的事件
void EventLoop::loop()
{
    while (!quit_) {
        auto activeChannels = ep_->poll();
        for (auto channel : activeChannels)
            channel->handleEvent();
    }
}

EventLoop::Epoll::Epoll()
{
    errif(-1 == (epfd_ = epoll_create1(0)), "epoll create error");
}

EventLoop::Epoll::~Epoll()
{
    if (epfd_ != -1)
        close(epfd_);
    epfd_ = -1;
}

void EventLoop::Epoll::ctl(int op, int fd, epoll_event* ev)
{
    errif(-1 == epoll_ctl(epfd_, op, fd, ev), "epoll_ctl error");
}


void set_ev(epoll_event& ev, int fd, int events)
{
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
}

void set_ev(epoll_event& ev, epoll_data_t  data, int events)
{
    bzero(&ev, sizeof(ev));
    ev.data = data;
    ev.events = events;
}