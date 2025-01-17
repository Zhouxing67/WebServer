#ifndef SERVER_H
#define SERVER_H
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"

/*
参见：
https://github.com/Wlgls/30daysCppWebServer中day6
 */
class Acceptor;
class Server {
private:
    EventLoop* loop_ = nullptr;
    Acceptor *accptor_ = nullptr;
public:
    Server(EventLoop* loop);
    ~Server();
    void newConnection(Socket *serv_sock);
    void handleEvent(int clntfd);
};
#endif