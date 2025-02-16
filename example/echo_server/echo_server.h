#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include "../../tcp/tcp.h"
#include <iostream>
#include <functional>
#include <arpa/inet.h>
#include <vector>

class EchoServer {
public:
    EchoServer(const char *ip, const int port);
    ~EchoServer();

    void start();
    void onConnection(const std::shared_ptr<TcpConnection> &conn);
    void onMessage(const std::shared_ptr<TcpConnection> &conn);

private:
    TcpServer server_;
};
#endif // ECHO_SERVER_H