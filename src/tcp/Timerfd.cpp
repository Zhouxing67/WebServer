#include "Timerfd.h"
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <strings.h>
#include <cassert>

//创建一个定时器，返回其描述符
int create_timerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
        TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
        throw std::runtime_error("Create timerfd error\n");
    return timerfd;

}

//读八个字节，用于激活定时器
void read_timerfd(TimeStamp now, int timerfd)
{
    uint64_t howmany;
    int n = ::read(timerfd, &howmany, sizeof(howmany));
    if (n != sizeof howmany) {
        throw std::runtime_error("TimerQueue::handleRead() does`t read 8 bytes");
    }
}

void reset_timerfd(TimeStamp when, int timerfd)
{
    // wake up loop by timerfd_settime()
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof (newValue));
    bzero(&oldValue, sizeof (oldValue));
    newValue.it_value = how_much_time_form_now(when);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    
    assert(ret != -1);
    (void) ret;
}
