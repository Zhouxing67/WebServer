#ifndef EPOLL_H
#define EPOLL_H

#define MAX_EVENTS 1024

#include <unistd.h>
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
    void ctl(int type, int fd, epoll_event* ev);
    vector<epoll_event> wait(int timeout = -1);
};

void set_ev(epoll_event& ev, int fd, int events);
#endif