#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <unordered_map>
#include <random>
#include <functional>
#include <memory>
#include "common.h"
#include "EventLoop.h"

using std::unordered_map;
using std::vector;
using std::random_device;
using std::uniform_int_distribution;
using std::function;
using std::unique_ptr;


class Channel;
class TcpConnection;
class ThreadPool;
class Acceptor;

class TcpServer
{
public:
    DISALLOW_COPY_AND_MOV(TcpServer);
    TcpServer(const char *ip, const int port);
    ~TcpServer();

    void Start();
    void set_connection_callback(function < void(TcpConnection *)> const &fn) { on_connect_ = fn; }
    void set_message_callback(function < void(TcpConnection *)> const &fn) { on_message_ = fn; }
    void handle_close(int fd);
    void handle_new_connection(int fd);

private:
    unique_ptr<EventLoop> main_reactor_;
    int next_conn_id_;
    unique_ptr<Acceptor> acceptor_;
    vector<unique_ptr<EventLoop>> sub_reactors_;
    unordered_map<int, TcpConnection *> connectionsMap_;
    unique_ptr<ThreadPool> thread_pool_;

    function<void(TcpConnection *)> on_connect_;
    function<void(TcpConnection *)> on_message_;

};

#endif