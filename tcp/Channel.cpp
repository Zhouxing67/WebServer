#include "Channel.h"

void Channel::enable_read()
{
    listen_events_ |= (EPOLLIN | EPOLLPRI);
    loop_->update_channel(this);
}

void Channel::enable_write()
{
    listen_events_ |= EPOLLOUT;
    loop_->update_channel(this);
}

void Channel::enable_ET()
{
    listen_events_ |= EPOLLET;
    loop_->update_channel(this);
}

void Channel::handle_event() const
{
    if (is_read_event() && read_callback_)
        read_callback_();
    if (is_write_event() && write_callback_)
        write_callback_();
}