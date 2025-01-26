#include "Acceptor.h"
#include "Channel.h"

Acceptor::Acceptor()
{ }

Acceptor::Acceptor(EventLoop *loop) : loop_(loop)
{
    sock_ = new Socket();
    inet_addr_ = new InetAddress("127.0.0.1", 8888);
    sock_->bind(inet_addr_);
    sock_->listen();

    function<void()> chl_cb = [this]() {this->accept_connection(); };
    chl_ = new Channel(loop_, sock_->getFd(), chl_cb);
    chl_->channel_ctl(EPOLLIN);
}

Acceptor::~Acceptor()
{ }

void Acceptor::accept_connection()
{
    new_conn_cb_(sock_);
}

