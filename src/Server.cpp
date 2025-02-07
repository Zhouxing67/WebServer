#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Server.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Connection.h"
#include "ThreadPool.h"


#define BUFSIZE 1024
using std::bind;

Server::Server(EventLoop *loop, function<void(Connection *)> func) : main_loop_(loop), on_conn_callback_(func)
{
    accptor_ = new Acceptor(main_loop_);
    function<void(Socket *)> accptor_cb = [this](Socket *sock) {this->new_connection(sock); };
    accptor_->set_cb(accptor_cb);

    concurrency_ = thread::hardware_concurrency();
    pool_ = new ThreadPool(concurrency_);
    sub_loops_ = vector<EventLoop *>(concurrency_, new EventLoop());
    for (auto sub_loop : sub_loops_) {
        auto task = [sub_loop] {sub_loop->loop(); };
        pool_->en_que(task);
    }
}

Server::~Server()
{
    for (auto sub_loop : sub_loops_)
        delete sub_loop;
    delete pool_;
    delete accptor_;
}

void Server::new_connection(Socket *serv_sock)
{
    InetAddress *clnt_addr = new InetAddress;
    Socket *clnt_sock = serv_sock->accept(clnt_addr);
    clnt_sock->setnonblocking();

    //初始化TCP连接，并将其随机分配到一个sub-reactor上
    unsigned int rd = random_();
    Connection *conn = new Connection(sub_loops_[rd], clnt_sock);
    conns_[clnt_sock->getFd()] = conn;

    function<void(Socket *sock)> cb = [this](Socket *sock) {this->release_connection(sock); };
    conn->set_del_conn_callback(cb);
    conn->set_on_conn_callback(on_conn_callback_);
}

void Server::release_connection(Socket *clnt_sock)
{
    int clnt_fd = clnt_sock->getFd();
    auto conn = conns_[clnt_fd];
    delete conn;
    conn = nullptr;
    conns_.erase(clnt_fd);
    printf("EOF, client fd %d disconnected\n", clnt_sock->getFd());
}




