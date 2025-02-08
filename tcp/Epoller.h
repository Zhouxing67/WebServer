#ifndef EPOLL_H
#define EPOLL_H
#define MAX_EVENTS 1024

#include "common.h"
#include "Util.h"
#include <sys/epoll.h>
#include <vector>
using std::vector;


class Channel;
class Epoller {
public:
    DISALLOW_COPY_AND_MOV(Epoller);
    Epoller();
    ~Epoller();

    //返回被触发的Channel
    vector<Channel *> poll(int timeout = -1);
    //更新Channel;
    void update_channel(Channel *chl);
    //移除channel
    void delete_channel(Channel *chl);

private:
    int epfd_ = -1;
    static epoll_event events[MAX_EVENTS];
    
    void ctl(int op, int fd, epoll_event *ev)
    {
        errif(-1 == epoll_ctl(epfd_, op, fd, ev), "epoll_ctl error");
    }
};
void set_ev(epoll_event &ev, epoll_data_t data, int events);

#endif