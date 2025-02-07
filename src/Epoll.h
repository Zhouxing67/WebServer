#ifndef EPOLL_H
#define EPOLL_H
#define MAX_EVENTS 1024

#include "Channel.h"
#include <sys/epoll.h>
#include <vector>
using std::vector;



class Epoll {
private:
    int epfd_ = -1;
    static epoll_event events[MAX_EVENTS];
public:
    Epoll();
    ~Epoll();
    void ctl(int op, int fd, epoll_event *ev);
    //返回被触发的Channel
    vector<Channel *> poll(int timeout = -1);
    //管理Channel;
    void channel_ctl(Channel *chl);
};


void set_ev(epoll_event &ev, epoll_data_t data, int events);

#endif