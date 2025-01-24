#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#define MAX_EVENTS 1024

#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

class Channel;
using std::vector;

void set_ev(epoll_event& ev, epoll_data_t data, int events);

class EventLoop {
private:
    class Epoll;
    Epoll* ep_ = nullptr;
    bool quit_ = false;
    //核心嵌套类，实现事件循环
    class Epoll {
    private:
        int epfd_ = -1;
        static epoll_event events[MAX_EVENTS];
    public:
        Epoll();
        ~Epoll();
        void ctl(int op, int fd, epoll_event* ev);
        //返回被触发的Channel
        vector<Channel*> poll(int timeout = -1);
        //管理Channel;
        void handle_channel(Channel* chl);
    };
public:
    EventLoop();
    ~EventLoop();
    void quit() { quit_ = true; }

    //监听channel
    void handle_channel(Channel* chl);
    void loop();
};
#endif