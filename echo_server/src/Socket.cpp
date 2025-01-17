#include "Socket.h"
#include "InetAddress.h"
#include "Util.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "socket create error");
}

Socket::Socket(int _fd) : fd_(_fd)
{
    errif(fd_ == -1, "socket create error");
}

Socket::~Socket()
{
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void Socket::bind(InetAddress* addr)
{
    errif(::bind(fd_, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
}

void Socket::listen()
{
    errif(::listen(fd_, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking()
{
    ::setnonblocking(fd_);
}

Socket* Socket::accept(InetAddress* addr)
{
    int clnt_sockfd = ::accept(fd_, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    printf("new client fd_ %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port));
    return new Socket(clnt_sockfd);
}
