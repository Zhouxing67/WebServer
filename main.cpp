#include "tcp/Acceptor.h"
#include "tcp/EventLoop.h"
#include "tcp/TcpServer.h"
#include "tcp/Buffer.h"
#include "tcp/ThreadPool.h"
#include "tcp/TcpConnection.h"
#include <string.h>
#include <iostream>
#include <functional>


int main(int argc, char *argv[])
{

    TcpServer *server = new TcpServer("127.0.0.1", 8888);

    server->set_message_callback([](shared_ptr<TcpConnection> conn) {
        if (conn->state() == TcpConnection::ConnectionState::Connected)
        {
            auto clnt_msg = conn->Rbuf_data();
            std::cout << "Message from client " << conn->id() << " is " << clnt_msg << std::endl;
            conn->Send(clnt_msg);
        }
        });
    server->Start();

    delete server;
    return 0;
}