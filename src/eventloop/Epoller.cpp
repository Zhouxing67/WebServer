#include "Epoller.h"
#include "Channel.h"
#include "Util.h"
#include <cstring>
#include <unistd.h>
#include "Logger.h"

void set_ev(epoll_event &ev, epoll_data_t data, int events) {
  bzero(&ev, sizeof(ev));
  ev.data = data;
  ev.events = events;
}

Epoller::Epoller() {
  events_ = new epoll_event[MAX_EVENTS];
  if (-1 == (epfd_ = epoll_create1(0)))
      LOG_ERROR << "epoll create error";
}

Epoller::~Epoller() {
  if (epfd_ != -1) {
    close(epfd_);
  }
  epfd_ = -1;
  delete events_;
}

void Epoller::update_channel(Channel *chl) {
  epoll_event ev;
  epoll_data_t data;
  data.ptr = chl;
  int fd = chl->fd();

  set_ev(ev, data, chl->listen_events());
  if (chl->is_InEpoll())
    ctl(EPOLL_CTL_MOD, fd, &ev);
  else {
    ctl(EPOLL_CTL_ADD, fd, &ev);
    chl->set_InEpoll();
  }
}

void Epoller::delete_channel(Channel *chl) {
  if (chl->is_InEpoll()) {
    chl->set_InEpoll(false);
    ctl(EPOLL_CTL_DEL, chl->fd(), nullptr);
  }
}

vector<Channel *> Epoller::poll(int timeout) {
  bzero(events_, sizeof(epoll_event));
  int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
  if (nfds == -1) {
    if (errno != EINTR)
        LOG_ERROR << "epoll wait error";
    else
        return {};
  }

  vector<Channel *> activeChannels(nfds, nullptr);
  for (int i = 0; i < nfds; i++) {
    Channel *activeCahnnel = static_cast<Channel *>(events_[i].data.ptr);
    activeCahnnel->set_ready_events(events_[i].events);
    activeChannels[i] = activeCahnnel;
  }
  return activeChannels;
}
