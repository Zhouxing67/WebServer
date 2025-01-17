#ifndef SERVER_H
#define SERVER_H
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"


class Server {
private:
    EventLoop* loop_ = nullptr;
public:
    Server(EventLoop* loop);
    ~Server();
    void newConnection(Socket *serv_sock);
    void handleEvent(int clntfd);
};
#endif