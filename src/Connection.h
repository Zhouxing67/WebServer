#ifndef CONNECTION_H
#define CONNECTION_H
#include <functional>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"
#include "InetAddress.h"

/*
在连接建立后，由Connection负责处理具体的业务逻辑
*/

using std::function;
class Connection
{
public:
    enum class State {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
private:
    friend class Server;

    EventLoop *loop_ = nullptr;
    Socket *clnt_sock_ = nullptr;
    Channel *chl_ = nullptr;
    Buffer *conn_buffer_ = nullptr;
    Buffer *write_buf_ = nullptr;
    Buffer *read_buf_ = nullptr;
    function<void(Socket *)> del_conn_callback_;
    function<void(Connection *)> on_conn_callback_;
    State state_ = State::Invalid;

    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();

    void release_conn();
    void set_del_conn_callback(function<void(Socket *)> cb) { del_conn_callback_ = cb; }
    void set_on_conn_callback(function<void(Connection *)> cb)
    {
        on_conn_callback_ = cb;
        chl_->set_callback([this] {on_conn_callback_(this); });
    }
    void handle_event();

public:
    Connection();
    Connection(EventLoop *loop, Socket *clnt_sock);
    Connection(const Connection &other) = delete;
    Connection &operator=(const Connection &other) = delete;
    ~Connection();

    //Write()函数表示将write_buffer_里的内容发送到clnt_sock_，发送后会清空写缓冲区
    void Write(bool is_block = true);
    //set_Wbuf 先清空写缓冲区，再往里冲入数据
    void set_Wbuf(const char *str) { write_buf_->set_buf(str); }
    //而Read()函数表示清空read_buffer_，然后将TCP缓冲区内的数据读取到读缓冲区
    void Read(bool is_block = true);

    void Close(){
        release_conn();
    }
    State state() { return state_; }
    const char *Rbuf_data() { return read_buf_->str_data(); }
    const char *Wbuf_data() { return write_buf_->str_data(); }
    int clnt_sockfd() { return clnt_sock_->getFd(); }
    void echo(int fd);
};

#endif