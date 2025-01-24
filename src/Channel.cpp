#include "Channel.h"

Channel::Channel() { };
Channel::Channel(EventLoop* loop, int fd, function<void()> cb) : loop_(loop), fd_(fd), callback_(cb)
{ }
Channel::~Channel()
{
    loop_ = nullptr;
}

void Channel::channel_ctl(unsigned int events)
{
    events_ = events;
    loop_->handle_channel(this);
}

void Channel::handle_event()
{
    callback_();
}