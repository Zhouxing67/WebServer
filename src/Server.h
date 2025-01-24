#ifndef SERVER_H
#define SERVER_H
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Connection.h"

#include <unordered_map>

using std::unordered_map;

/*
参见：
https://github.com/Wlgls/30daysCppWebServer中day6
 */
class Acceptor;
class Server {
private:
    EventLoop* loop_ = nullptr;
    Acceptor *accptor_ = nullptr;
    unordered_map<int, Connection *> conns_;
public:
    Server(EventLoop* loop);
    ~Server();
    void newConnection(Socket *serv_sock);
    void release_conn(Socket *clnt_sock);
};
#endif