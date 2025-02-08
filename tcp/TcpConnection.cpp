#include <memory>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <sys/socket.h>
#include <cstring>

#include "TcpConnection.h"
#include "Buffer.h"
#include "Channel.h"
#include "common.h"
#include "EventLoop.h"

#define BUF_SIZE 1024


TcpConnection::TcpConnection(EventLoop *loop, int connfd, int connid) : connfd_(connfd), connid_(connid), loop_(loop)
{

    if (loop != nullptr) {
        channel_ = std::make_unique<Channel>(connfd, loop);
        channel_->enable_ET();
        channel_->set_read_callback([this] {this->handle_message(); });
        channel_->enable_read();
    }
    read_buf_ = std::make_unique<Buffer>();
    send_buf_ = std::make_unique<Buffer>();
}

TcpConnection::~TcpConnection()
{
    ::close(connfd_);
}

void TcpConnection::Send(const std::string &msg)
{
    set_send_buf(msg.c_str());
    Write();
}

void TcpConnection::Send(const char *msg)
{
    set_send_buf(msg);
    Write();
}

void TcpConnection::handle_message()
{
    Read();
    if (state_ == ConnectionState::Disconected)
        return;
    if (on_message_)
        on_message_(this);
}

void TcpConnection::handle_close()
{
    if (state_ != ConnectionState::Disconected)
    {
        state_ == ConnectionState::Disconected;
        if (on_close_)
            on_close_(connfd_);
    }
}

void TcpConnection::Read()
{
    read_buf_->clear();
    ReadNonBlocking();
}

void TcpConnection::Write()
{
    WriteNonBlocking();
    send_buf_->clear();
}


void TcpConnection::ReadNonBlocking()
{
    char buf[BUF_SIZE];
    while (true) {
        bzero(buf, sizeof(buf));
        ssize_t bytes_read = read(connfd_, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buf_->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR) {
            //std::cout << "continue reading" << std::endl;
            continue;
        }
        else if ((bytes_read == -1) && (
            (errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            break;
        }
        else if (bytes_read == 0) {//
            handle_close();
            break;
        }
        else {
            handle_close();
            break;
        }
    }
}

void TcpConnection::WriteNonBlocking()
{
    char buf[send_buf_->size()];
    memcpy(buf, send_buf_->str_data(), send_buf_->size());
    int data_size = send_buf_->size();
    int data_left = data_size;

    while (data_left > 0) {
        ssize_t bytes_write = write(connfd_, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR) {
            //std::cout << "continue writing" << std::endl;
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            handle_close();
            break;
        }
        data_left -= bytes_write;
    }
}
