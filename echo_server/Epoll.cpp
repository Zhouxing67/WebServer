#include "Epoll.h"
#include "Util.h"
#include <string.h>
#include "Channel.h"
epoll_event Epoll::events[MAX_EVENTS];

Epoll::Epoll()
{
    errif(-1 == (epfd_ = epoll_create1(0)), "epoll create error");
}

Epoll::~Epoll()
{
    if (epfd_ != -1)
        close(epfd_);
    epfd_ = -1;
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

void Epoll::handleChannel(Channel* chl)
{
    epoll_event ev;
    epoll_data_t data;
    data.ptr = chl;
    int fd = chl->getFD();
    set_ev(ev, data, chl->events());
    if (chl->is_ctl())
        ctl(EPOLL_CTL_MOD, fd, &ev);
    else 
        ctl(EPOLL_CTL_ADD, fd, &ev);
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