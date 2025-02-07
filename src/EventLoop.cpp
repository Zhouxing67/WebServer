#include "EventLoop.h"
#include "Epoll.h"
#include "Util.h"
#include <string.h>
#include "Channel.h"


EventLoop::EventLoop() : ep_(new Epoll())
{}

EventLoop::~EventLoop()
{
    delete ep_;
    ep_ = nullptr;
}

void EventLoop::channel_ctl(Channel *chl)
{
    ep_->channel_ctl(chl);
}

//每次事件循环，获取被触发的channel列表，让每一个活动channel处理自己的事件
void EventLoop::loop()
{
    while (!quit_) {
        auto activeChannels = ep_->poll();
        for (auto channel : activeChannels)
            channel->handle_event();
    }
}
