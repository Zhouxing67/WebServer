#include "Connection.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "Util.h"

#define BUFSIZE 1024
Connection::Connection()
{ }

//与Acceptor类相似，在构造函数中完成Channel向epoll的注册；
Connection::Connection(EventLoop *loop, Socket *clnt_sock) : loop_(loop), clnt_sock_(clnt_sock)
{
    conn_buffer_ = new Buffer;
    int clnt_fd = clnt_sock->getFd();
    function<void()> chl_cb = [this]() {
        this->handle_event();
        };
    chl_ = new Channel(loop_, clnt_fd, chl_cb);
    chl_->channel_ctl();
}

Connection::~Connection()
{
    delete chl_;
    delete clnt_sock_;
    delete conn_buffer_;

    conn_buffer_ = nullptr;
    chl_ = nullptr;
    clnt_sock_ = nullptr;
}

void Connection::release_conn()
{
    release_cb_(clnt_sock_);
}

void Connection::echo(int fd)
{
    char buf[BUFSIZE];
    while (true) {    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            conn_buffer_->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR) {  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {//非阻塞IO，这个条件表示数据全部读取完毕
            const char *data = conn_buffer_->str_data();
            size_t size = conn_buffer_->size();
            printf("message from client fd %d: %s\n", fd, data);
            errif((write(fd, data, size) != size), "write error!!!");
            conn_buffer_->clear();
            break;
        }
        else if (bytes_read == 0) {  //EOF，客户端断开连接
            release_conn();
            close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}
void Connection::handle_event()
{
    this->echo(clnt_sock_->getFd());
}