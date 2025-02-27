#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
namespace CurrentThread
{
    extern __thread int t_cachedTid;
    extern __thread char t_formattedTid[32];
    extern __thread int t_formattedTidLength;

    inline const char *tidString() { return t_formattedTid; }
    inline int tidStringLength() { return t_formattedTidLength; }
    
    pid_t gettid();
    void CacheTid();
    inline int tid()
    {
        if (__builtin_expect(t_cachedTid == 0, 0)) {//期望 (t_cacheTid == 0) 的结果未0
            CacheTid();
        }
        return t_cachedTid;
    }
    

}
#endif