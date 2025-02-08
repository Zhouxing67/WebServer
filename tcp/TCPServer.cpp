#include <unistd.h>
#include <iostream>
#include <assert.h>

#include "TcpServer.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "ThreadPool.h"
#include "common.h"

using std::make_unique;
using std::cout;
using std::endl;

TcpServer::TcpServer(const char *ip, const int port) : next_conn_id_(1)
{
    main_reactor_ = make_unique<EventLoop>();
    acceptor_ = make_unique<Acceptor>(main_reactor_.get(), ip, port);
    function<void(int)> cb = [this](int fd) {handle_new_connection(fd); };
    acceptor_->set_new_conn_callback(cb);

    unsigned int size = thread::hardware_concurrency();
    thread_pool_ = make_unique<ThreadPool>(size);

    for (size_t i = 0; i < size; ++i) {
        unique_ptr<EventLoop> sub_reactor = make_unique<EventLoop>();
        sub_reactors_.push_back(std::move(sub_reactor));
    }
}

TcpServer::~TcpServer() { }

void TcpServer::Start()
{
    for (auto &sub_reactor: sub_reactors_) {
        function<void()> sub_loop = [&sub_reactor] {sub_reactor->loop(); };
        thread_pool_->en_que(std::move(sub_loop));
    }
    std::cout << "Server Start !!!" << std::endl;
    main_reactor_->loop();
}

void TcpServer::handle_new_connection(int fd)
{
    assert(fd != -1);
    cout << "New connection fd: " << fd << endl;
    uint64_t random = fd % sub_reactors_.size();

    TcpConnection *conn = new TcpConnection(sub_reactors_[random].get(), fd, next_conn_id_);
    function<void(int)> cb = [this](int fd) {handle_close(fd); };

    conn->set_close_callback(cb);
    conn->set_message_callback(on_message_);
    connectionsMap_[fd] = conn;

    // 分配id
    ++next_conn_id_;
    if (next_conn_id_ == 1000) 
        next_conn_id_ = 1;
}

void TcpServer::handle_close(int fd)
{
    auto it = connectionsMap_.find(fd);
    assert(it != connectionsMap_.end());
    TcpConnection *conn = connectionsMap_[fd];
    connectionsMap_.erase(fd);
    ::close(fd);
    cout << "Close " << conn->id() << " connection!!! " << endl;
    conn = nullptr;
}

