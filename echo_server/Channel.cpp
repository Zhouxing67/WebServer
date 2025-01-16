#include "Channel.h"

Channel::Channel() { };
Channel::Channel(Epoll* ep, int fd) : ep_(ep), fd_(fd)
{ }
Channel::~Channel()
{
    ep_ = nullptr;
}

void Channel::channel_ctl(int events)
{
    events_ = events;
    ep_->handleChannel(this);
}