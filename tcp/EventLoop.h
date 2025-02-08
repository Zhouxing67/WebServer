#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/epoll.h>
#include <vector>
#include <memory>
#include "common.h"

using std::vector;
using std::unique_ptr;

class Channel;
class Epoller;
class EventLoop
{
public:
    DISALLOW_COPY_AND_MOV(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop() const;
    void update_channel(Channel *ch) const;
    void delete_channel(Channel *ch) const;

private:
    unique_ptr<Epoller> poller_;
};
#endif