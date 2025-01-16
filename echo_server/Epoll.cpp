#include "Epoll.h"
#include "Util.h"
#include <string.h>
epoll_event Epoll::events[MAX_EVENTS];

void set_ev(epoll_event& ev, int fd, int events)
{
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
}


Epoll::Epoll()
{
    errif(-1 == (epfd_ = epoll_create1(0)), "epoll create error");
}

Epoll::~Epoll()
{
    if (epfd_ != -1)
        close(epfd_);
}

void Epoll::ctl(int op, int fd, epoll_event* ev)
{
  
    errif(-1 == epoll_ctl(epfd_, op, fd, ev), "epoll_ctl error");
}

vector<epoll_event> Epoll::wait(int timeout)
{
    bzero(events, sizeof(events));
    int nfds = epoll_wait(epfd_, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll_wait error");
    return vector<epoll_event>(std::begin(events), std::begin(events) + nfds);
}