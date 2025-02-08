#ifndef CONNECTION_H
#define CONNECTION_H

#include "common.h"
#include <functional>
#include <memory>
#include <string>
#include "Buffer.h"
#include "EventLoop.h"

using std::function;
using std::unique_ptr;

class TcpConnection
{
public:
    enum ConnectionState
    {
        Invalid = 1,
        Connected,
        Disconected
    };

    DISALLOW_COPY_AND_MOV(TcpConnection);

    
    TcpConnection(EventLoop *loop, int connfd, int connid);
    ~TcpConnection();

     // 关闭时的回调函数
    void set_close_callback(std::function<void(int)> const &fn) { on_close_ = fn; }
    // 接受到信息的回调函数                                  
    void set_message_callback(std::function<void(TcpConnection *)> const &fn) { on_message_ = fn; }


    // 先清空写缓冲区，再往里冲入数据
    void set_send_buf(const char *str) { send_buf_->set_buf(str); }
    const char *Rbuf_data() const { return read_buf_->str_data(); }
    const char *Wbuf_data() const { return send_buf_->str_data(); }
    // 读操作
    //Read()函数表示清空read_buffer_，然后将TCP缓冲区内的数据读取到读缓冲区
    void Read();
    // 写操作
    //Write()函数表示将write_buffer_里的内容发送到clnt_sock_，发送后会清空写缓冲区
    void Write();

    // 输出信息
    void Send(const std::string &msg);
    void Send(const char *msg, int len);
    void Send(const char *msg);

    // 当接收到信息时，进行回调
    void handle_message();

    // 当TcpConnection发起关闭请求时，进行回调，释放相应的socket.
    void handle_close();


    ConnectionState state() const { return state_; }
    EventLoop *loop() const { return loop_; }
    int fd() const { return connfd_; }
    int id() const { return connid_; }

private:
    // 该连接绑定的Socket
    int connfd_;
    int connid_;
    // 连接状态
    ConnectionState state_;
    EventLoop *loop_;
    unique_ptr<Channel> channel_;
    unique_ptr<Buffer> read_buf_;
    unique_ptr<Buffer> send_buf_;
    function<void(int)> on_close_;
    function<void(TcpConnection *)> on_message_;

    void ReadNonBlocking();
    void ReadBLocking();
    void WriteNonBlocking();
    void WriteBlocking();
};
#endif