#include "Server.h"
#include "InetAddress.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 1024
using std::bind;

Server::Server(EventLoop* loop) : loop_(loop)
{
    Socket *serv_sock = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    function<void()> cb = [this, serv_sock]() {this->newConnection(serv_sock); };
    Channel* serv_chl = new Channel(loop, serv_sock->getFd(), cb);
    //serv_chl->setcallback(cb);
    serv_chl->channel_ctl();
}

Server::~Server()
{
}

void Server::newConnection(Socket* serv_sock)
{
    InetAddress* clnt_addr = new InetAddress;
    Socket *clnt_sock = serv_sock->accept(clnt_addr);
    clnt_sock->setnonblocking();

    function<void()> cb = [clnt_sock, this]() {this->handleEvent(clnt_sock->getFd());};
    Channel* clnt_chl = new Channel(loop_, clnt_sock->getFd(), cb);
   // clnt_chl->setcallback(cb);
    clnt_chl->channel_ctl();
}

void Server::handleEvent(int fd)
{
    char buf[BUFSIZE];
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

