#ifndef TIME_FD_H
#define TIME_FD_H

#include<sys/timerfd.h>
#include<TimeStamp.h>

//创建timerfd
int create_timerfd();

//读取timerfd事件
void read_timerfd(TimeStamp now, int timerfd);

//重新设置timerfd超时时间，关注新的定时任务
void reset_timerfd(TimeStamp when, int timerfd);
#endif
