#ifndef COMMON_H
#define COMMON_H

#define DISALLOW_COPY(cname)       \
    cname(const cname &) = delete; \
    cname &operator=(const cname &) = delete;

#define DISALLOW_MOV(cname)        \
    cname(cname &&) = delete;      \
    cname &operator=(cname &&) = delete; 

#define DISALLOW_COPY_AND_MOV(cname)     \
    DISALLOW_COPY(cname)                 \
    DISALLOW_MOV(cname) 
#endif