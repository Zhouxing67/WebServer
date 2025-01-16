#ifndef EPOLL_H
#define EPOLL_H

#define MAX_EVENTS 1024

#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

class Channel;
using std::vector;
class Epoll {
private:
    int epfd_ = -1;
    static epoll_event events[MAX_EVENTS];
public:
    Epoll();
    ~Epoll();
    void ctl(int op, int fd, epoll_event* ev);
    vector<epoll_event> wait(int timeout = -1);
    //管理Channel;
    void handleChannel(Channel* chl);
};

void set_ev(epoll_event& ev, int fd, int events);
void set_ev(epoll_event& ev, epoll_data_t data, int events);

#endif