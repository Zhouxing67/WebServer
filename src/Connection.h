#ifndef CONNECTION_H
#define CONNECTION_H

/*
在连接建立后，由Connection负责处理具体的业务逻辑
*/

#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Buffer.h"
class Connection
{
private:
    EventLoop *loop_ = nullptr;
    Socket *clnt_sock_ = nullptr;
    Channel *chl_ = nullptr;
    Buffer *conn_buffer_ = nullptr;
    function<void(Socket *)> release_cb_;
public:
    Connection();
    Connection(EventLoop *loop, Socket *clnt_sock);
    Connection(const Connection &other) = delete;
    Connection &operator=(const Connection &other) = delete;
    ~Connection();

    void release_conn();
    void set_cb(function<void(Socket *)> cb) { release_cb_ = cb; }
    
    void handle_event();
    void echo(int fd);
};

#endif