#include "Server.h"
#include "InetAddress.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Acceptor.h"


#define BUFSIZE 1024
using std::bind;

Server::Server(EventLoop *loop) : loop_(loop)
{
    accptor_ = new Acceptor(loop_);
    function<void(Socket *)> accptor_cb = [this](Socket *sock) {this->newConnection(sock); };
    accptor_->set_cb(accptor_cb);
}

Server::~Server()
{ }

void Server::newConnection(Socket *serv_sock)
{
    InetAddress *clnt_addr = new InetAddress;
    Socket *clnt_sock = serv_sock->accept(clnt_addr);
    clnt_sock->setnonblocking();

    Connection *conn = new Connection(loop_, clnt_sock);
    conns_[clnt_sock->getFd()] = conn;
    function<void(Socket *sock)> cb = [this](Socket *sock) {this->release_conn(sock); };
    conn->set_cb(cb);
}

void Server::release_conn(Socket *clnt_sock)
{
    int clnt_fd = clnt_sock->getFd();
    auto conn = conns_[clnt_fd];
    delete conn;
    conns_.erase(clnt_fd);
    printf("EOF, client fd %d disconnected\n", clnt_sock->getFd());
}




