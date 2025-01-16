#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>
#include "Util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"

char buf[READ_BUFFER];

InetAddress serv_addr("127.0.0.1", 8888), clnt_addr;
Socket serv_sock;
struct epoll_event ev;
Epoll epoll;

void handing(int fd);
int main()
{
    serv_sock.bind(&serv_addr);
    serv_sock.listen();
    serv_sock.setnonblocking();
    set_ev(ev, serv_sock.getFd(), EPOLLIN | EPOLLET);
    epoll.ctl(EPOLL_CTL_ADD, serv_sock.getFd(), &ev);
    while (true) {
        auto events = epoll.wait();
        for (auto& event : events) {
            if (int fd = event.data.fd; fd == serv_sock.getFd()) {//新客户端连接
                printf("新客户端连接\n");
                int clnt_sockfd = serv_sock.accept(&clnt_addr);
                setnonblocking(clnt_sockfd);
                set_ev(ev, clnt_sockfd, EPOLLIN | EPOLLET);
                epoll.ctl(EPOLL_CTL_ADD, clnt_sockfd, &ev);
            }
            else if (event.events & EPOLLIN) {      //可读事件
                handing(fd);
            }
            else {         //其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
    }
    return 0;
}

void handing(int fd)
{
    while (true) {    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", fd, buf);
            write(fd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR) {  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if (bytes_read == 0) {  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}