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

TcpServer::TcpServer(const char *ip, const int port)
{
    main_reactor_ = make_unique<EventLoop>();

    acceptor_ = make_unique<Acceptor>(main_reactor_.get(), ip, port);
    acceptor_->set_new_conn_callback([this](int fd) {handle_new_connection(fd); });

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
    for (auto &sub_reactor : sub_reactors_)
        thread_pool_->en_que([&sub_reactor] {sub_reactor->loop(); });

    print_if(1, "Main reactor loop");
    main_reactor_->loop();
}

void TcpServer::handle_new_connection(int fd)
{
    assert(fd != -1);
    static int next_conn_id_ = 1;

    //cout << "New connection fd: " << fd << endl;
    uint64_t random = fd % sub_reactors_.size();
    shared_ptr<TcpConnection> conn = make_shared<TcpConnection>(sub_reactors_[random].get(), fd, next_conn_id_);

    conn->set_connect_callback(on_connect_);
    conn->set_close_callback([this](const shared_ptr<TcpConnection> &conn) {handle_close(conn); });
    conn->set_message_callback(on_message_);

    {
        lock_guard<mutex> lock(mut_);
    }
    connectionsMap_[fd] = conn;

    // 分配id
    ++next_conn_id_;
    if (next_conn_id_ == 1000)
        next_conn_id_ = 1;
    conn->establish_connection();
}

void TcpServer::handle_close(const shared_ptr<TcpConnection> &conn)
{
    //由main_reactor_来执行`HandleCloseInLoop`函数，来保证线程安全
    //此时main_reactor_负责既负责新建连接，又负责关闭连接
    main_reactor_->run_one_func([this, conn] {handle_close_in_loop(conn); });
}

void TcpServer::handle_close_in_loop(const shared_ptr<TcpConnection> &conn)
{
    cout << "Close connection "<<conn->id()  << " Current thread id: " << CurrentThread::tid() << endl;
    int fd = conn->fd();
    auto it = connectionsMap_.find(fd);
    assert(it != connectionsMap_.end());

    connectionsMap_.erase(connectionsMap_.find(fd));
    EventLoop *loop = conn->loop();
    loop->add_one_func([conn] {conn->delete_connection(); });
}

