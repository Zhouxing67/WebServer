#ifndef ACCEPTOR_H
#define ACCEPTOR_H
/*
对于任何事件，无论提供什么样的服务，首先需要做的事：
1.调用accept()函数接受这个TCP连接 -> 2.将socket文件描述符添加到epoll，监听IO ->
3.当这个IO口有事件发生的时候，再对此TCP连接提供相应的服务;

Acceptor 类的职责可以看作是 网络连接的接受者。
它专注于 TCP 连接的监听和接收，并通过回调将连接交给其他组件（如 Server 类）进行处理。
这样可以确保 Acceptor 类的设计简单且专注于其核心职责，不涉及处理连接的具体细节。
*/
#include <functional>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include "Channel.h"
#include "EventLoop.h"
#include "common.h"

using std::function;
using std::unique_ptr;

class Acceptor {
public:
    DISALLOW_COPY_AND_MOV(Acceptor);
    Acceptor(EventLoop *loop, const char* ip4, uint16_t port);
    Acceptor() = default;
    ~Acceptor();

    void set_new_conn_callback(const function<void(int)> &fn) { new_conn_callback_ = fn; }
    void set_new_conn_callback(function<void(int)> &&fn) { new_conn_callback_ = std::move(fn); }
    
    void accept_connection();
    void Create();
    void Bind(const char *ip, const int port);
    void Listen();

private:
    EventLoop *loop_ = nullptr;
    unique_ptr<Channel> chl_;
    int listenfd_ = -1;
    function<void(int)> new_conn_callback_; //接受TCP连接的具体行为，通过回调函数将其具体实现委派给Server类
};
#endif