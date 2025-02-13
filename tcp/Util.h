#ifndef UTIL_H
#define UTIL_H
#define READ_BUFFER 1024

void errif(bool condition, const char *err_msg);
void print_if(bool cond, const char *msg);
void setnonblocking(int fd);
#endif