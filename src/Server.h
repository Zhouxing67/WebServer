#ifndef SERVER_H
#define SERVER_H
#include <vector>
#include <unordered_map>
#include <random>
#include <functional>

using std::unordered_map;
using std::vector;
using std::random_device;
using std::uniform_int_distribution;
using std::function;

class EventLoop;
class Channel;
class Socket;
class Connection;
class ThreadPool;
/*
参见：
https://github.com/Wlgls/30daysCppWebServer中day6
 */

class Acceptor;
class Server {
private:
    EventLoop *main_loop_ = nullptr;
    vector<EventLoop *> sub_loops_;
    Acceptor *accptor_ = nullptr;
    unordered_map<int, Connection *> conns_;
    ThreadPool *pool_ = nullptr;
    unsigned int concurrency_;
    function<void(Connection *)> on_conn_callback_;

    unsigned int random_()
    {
        random_device rd;
        std::mt19937 gen(rd());
        uniform_int_distribution<> dis{ 0, concurrency_ };
        return dis(gen);
    }

    void new_connection(Socket *serv_sock);
    void release_connection(Socket *clnt_sock);

public:
    Server(EventLoop *loop, function<void(Connection *)> on_conn_callback = [](Connection *){});
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;
    ~Server();

    void on_connection(function<void(Connection *)> cb) { on_conn_callback_ = cb; }

};
#endif