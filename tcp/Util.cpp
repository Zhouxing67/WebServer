#include "Util.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

using std::cout;
using std::endl;

void errif(bool condition, const char* err_msg)
{
    if (condition) {
        perror(err_msg);
        exit(EXIT_FAILURE);
    }
}

void print_if(bool cond, const char* msg)
{
    {
#ifdef _DEBUG        // 如果定义了 _DEBUG 宏
        if (cond) {      // 如果条件为真
            cout << "Debug: " << msg << endl;
        }
#endif               // 如果没有定义 _DEBUG 宏，则函数体为空
    }
}

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}