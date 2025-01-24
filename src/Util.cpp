#include "Util.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>


void errif(bool condition, const char* err_msg)
{
    if (condition) {
        perror(err_msg);
        exit(EXIT_FAILURE);
    }
}
void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}