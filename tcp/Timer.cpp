#include "Timer.h"

void Timer::restart(TimeStamp when)
{
    if (repeat_)
        expiration_ = when + interval_;
    else
        expiration_ = TimeStamp::Invalid();
}

void Timer::restart()
{
    restart(TimeStamp::Now());
}
