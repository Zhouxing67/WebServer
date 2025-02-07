#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "Connection.h"
#include "Util.h"
#include <cassert>


#define BUFSIZE 1024
Connection::Connection()
{ }

//与Acceptor类相似，在构造函数中完成Channel向epoll的注册；
Connection::Connection(EventLoop *loop, Socket *clnt_sock) : loop_(loop), clnt_sock_(clnt_sock)
{
    read_buf_ = new Buffer();
    write_buf_ = new Buffer();
    state_ = State::Connected;

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
    delete write_buf_;
    delete read_buf_;

    chl_ = nullptr;
    clnt_sock_ = nullptr;
}

void Connection::release_conn()
{
    del_conn_callback_(clnt_sock_);
}

void Connection::Read(bool is_block)
{
    assert(state_ == State::Connected);
    read_buf_->clear();
    if (is_block)
        ReadBlocking();
    else
        ReadNonBlocking();
}
void Connection::Write(bool is_block)
{
    assert(state_ == State::Connected);
    if (is_block)
        WriteBlocking();
    else
        WriteNonBlocking();
    write_buf_->clear();
}

void Connection::handle_event() { }


void Connection::ReadNonBlocking()
{
    int sockfd = clnt_sock_->getFd();
    char buf[1024];  // 这个buf大小无所谓
    while (true) {   // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buf_->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断、继续读取
            printf("continue reading\n");
            continue;
        }
        else if (bytes_read == -1 &&
            ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if (bytes_read == 0) {  // EOF，客户端断开连接
            printf("read EOF, client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            break;
        }
        else {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
    }
}
void Connection::WriteNonBlocking()
{
    int sockfd = clnt_sock_->getFd();
    size_t size = write_buf_->size();

    char buf[size];
    memcpy(buf, write_buf_->str_data(), size);

    int data_size = size;
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::ReadBlocking() { }
void Connection::WriteBlocking() { }
void Connection::echo(int fd) { }


