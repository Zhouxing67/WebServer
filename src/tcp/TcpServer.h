#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <unordered_map>
#include <random>
#include <functional>
#include <memory>
#include "common.h"
#include "EventLoop.h"
#include "TcpConnection.h"

using std::unordered_map;
using std::vector;
using std::random_device;
using std::uniform_int_distribution;
using std::function;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;


class Channel;
class ThreadPool;
class Acceptor;
class EventLoopThreadPool;
using TcpConnectionCallbcak = TcpConnection::TcpConnectionCallback;

class TcpServer
{
public:
    DISALLOW_COPY_AND_MOV(TcpServer);
    TcpServer(const char *ip, const int port);
    ~TcpServer();

    void Start();

    void set_active_time(Second second = 30.0) { active_time_ = second; }
    //设置建立连接时的业务逻辑
    void set_connection_callback(TcpConnectionCallbcak fn) { on_connect_ = std::move(fn); }
    //设置连接的业务逻辑
    void set_message_callback(TcpConnectionCallbcak fn) { on_message_ = std::move(fn); }

    void handle_shutdown( int fd);
    void handle_close(const shared_ptr<TcpConnection> &conn);
    void handle_close_in_loop(const shared_ptr<TcpConnection> &conn);
    void handle_new_connection(int fd);

private:
    EventLoop *main_reactor_ = nullptr;
    Second active_time_ = -1.0; //此时服务端不主动断开连接

    unique_ptr<Acceptor> acceptor_;
    vector<unique_ptr<EventLoop>> sub_reactors_;
    unordered_map<int, shared_ptr<TcpConnection>>connectionsMap_;
    unique_ptr<EventLoopThreadPool> thread_pool_;

    TcpConnectionCallbcak on_message_;
    TcpConnectionCallbcak on_connect_;

    //生成TcpConnection连接id
    int get_conn_id()
    {
        static int id = 0;
        if(id == 1000)
            id = 0;
        return id++;
    }

};

#endif