#include "Channel.h"
#include "Epoller.h"
#include "EventLoop.h"
#include "Util.h"
#include <string.h> 
#include <iostream>


EventLoop::EventLoop()
{
    poller_ = std::make_unique<Epoller>();
}

EventLoop::~EventLoop(){ }

void EventLoop::update_channel(Channel *chl) const
{
    poller_->update_channel(chl);
}

void EventLoop::delete_channel(Channel *chl) const
{
    poller_->delete_channel(chl);
}

//每次事件循环，获取被触发的channel列表，让每一个活动channel处理自己的事件
void EventLoop::loop() const
{
    while (true) {
        auto activeChannels = poller_->poll();
        for (auto channel : activeChannels)
            channel->handle_event();
    }
}
