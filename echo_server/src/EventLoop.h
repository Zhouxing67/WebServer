#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#define MAX_EVENTS 1024

#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

class Channel;
using std::vector;

void set_ev(epoll_event& ev, int fd, int events);
void set_ev(epoll_event& ev, epoll_data_t data, int events);

class EventLoop {
private:
    class Epoll;
    Epoll* ep_ = nullptr;
    bool quit_ = false;

    class Epoll {
    private:
        int epfd_ = -1;
        static epoll_event events[MAX_EVENTS];
    public:
        Epoll();
        ~Epoll();
        void ctl(int op, int fd, epoll_event* ev);
        vector<Channel*> poll(int timeout = -1);
        //管理Channel;
        void handleChannel(Channel* chl);
    };
public:
    EventLoop();
    ~EventLoop();
    //监听channel
    void handleChannel(Channel* chl);
    void loop();
    void quit() { quit_ = true; }
};
#endif