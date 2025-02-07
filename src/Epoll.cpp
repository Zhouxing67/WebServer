#include "Epoll.h"
#include <cstring>
#include "Util.h"
#include <unistd.h>

epoll_event Epoll::events[MAX_EVENTS];

void set_ev(epoll_event &ev, epoll_data_t  data, int events)
{
    bzero(&ev, sizeof(ev));
    ev.data = data;
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
    epfd_ = -1;
}

void Epoll::channel_ctl(Channel *chl)
{
    epoll_event ev;
    epoll_data_t data;
    data.ptr = chl;

    int fd = chl->fd();
    set_ev(ev, data, chl->events());
    if (chl->is_ctl())
        ctl(EPOLL_CTL_MOD, fd, &ev);
    else {
        ctl(EPOLL_CTL_ADD, fd, &ev);
        chl->is_ctl();
    }
}

vector<Channel *> Epoll::poll(int timeout)
{
    bzero(events, sizeof(events));
    int nfds = epoll_wait(epfd_, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll_wait error");
    
    vector<Channel *> activeCahnnels(nfds, nullptr);
    for (int i = 0; i < nfds; i++)
        activeCahnnels[i] = static_cast<Channel *>(events[i].data.ptr);
    return activeCahnnels;
}


void Epoll::ctl(int op, int fd, epoll_event *ev)
{
    errif(-1 == epoll_ctl(epfd_, op, fd, ev), "epoll_ctl error");
}