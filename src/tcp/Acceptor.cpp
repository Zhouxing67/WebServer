#include "Acceptor.h"
#include "Logger.h"
#include <cassert>

using std::make_unique;

Acceptor::Acceptor(EventLoop *loop, const char *ip4, uint16_t port) : loop_(loop)
{
    Create();
    Bind(ip4, port);
    Listen();

    chl_ = make_unique<Channel>(listenfd_, loop_);
    chl_->set_read_callback([this] {accept_connection(); });
    chl_->enable_read(); 
}

Acceptor::~Acceptor()
{ }

void Acceptor::accept_connection()
{
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    assert(listenfd_ != -1);

    int clnt_fd = ::accept4(listenfd_, (struct sockaddr *)&client, &client_addrlength, SOCK_NONBLOCK | SOCK_CLOEXEC);
    
    if (new_conn_callback_)
        new_conn_callback_(clnt_fd);
}

void Acceptor::Create()
{
    assert(listenfd_ == -1);
    listenfd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (listenfd_ == -1) {
        LOG_ERROR << "Failed to create socket";
    }
}

void Acceptor::Bind(const char *ip, const int port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if (::bind(listenfd_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        LOG_ERROR << "Failed to Bind : " << ip <<":" <<port ;
    }
}

void Acceptor::Listen()
{
    assert(listenfd_ != -1);
    if (::listen(listenfd_, SOMAXCONN) == -1) {
        LOG_ERROR << "Failed to Listen";
    }
}
