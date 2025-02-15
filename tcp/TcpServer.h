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


class Channel;
class ThreadPool;
class Acceptor;
using TcpConnectionCallbcak = TcpConnection::TcpConnectionCallback; 

class TcpServer
{
public:
    DISALLOW_COPY_AND_MOV(TcpServer);
    TcpServer(const char *ip, const int port);
    ~TcpServer();

    void Start();
    void set_connection_callback(TcpConnectionCallbcak fn) { on_connect_ = std::move(fn); }
    void set_message_callback(TcpConnectionCallbcak fn) { on_message_ = std::move(fn); }
    
    void handle_close(const shared_ptr<TcpConnection> &conn);
    void handle_new_connection(int fd);

private:
    unique_ptr<EventLoop> main_reactor_;
    
    unique_ptr<Acceptor> acceptor_;
    vector<unique_ptr<EventLoop>> sub_reactors_;
    unordered_map<int, shared_ptr<TcpConnection>>connectionsMap_;
    unique_ptr<ThreadPool> thread_pool_;

    TcpConnectionCallbcak on_message_;
    TcpConnectionCallbcak on_connect_;
    mutex mut_;

};

#endif