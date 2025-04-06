#include <assert.h>
#include <unistd.h>

#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "Logger.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "common.h"

using std::make_shared;
using std::make_unique;

TcpServer::TcpServer(const char *ip, const int port)
{
    main_reactor_ = new EventLoop();

    acceptor_ = make_unique<Acceptor>(main_reactor_, ip, port);
    acceptor_->set_new_conn_callback([this](int fd) { handle_new_connection(fd); });

    thread_pool_ = make_unique<EventLoopThreadPool>(main_reactor_);
}

TcpServer::~TcpServer()
{
    delete main_reactor_;
    main_reactor_ = nullptr;
}

void TcpServer::Start()
{
    thread_pool_->start();
    main_reactor_->run_every(15.0, [] { LOG_INFO << "HELLO! 15-SECOND HAVE PASSED"; });
    main_reactor_->loop();
}

void TcpServer::handle_new_connection(int fd)
{
    assert(fd != -1);
    int next_conn_id_ = get_conn_id();

    // cout << "New connection fd: " << fd << endl;
    EventLoop *sub_reactors_ = thread_pool_->get_next_loop();
    shared_ptr<TcpConnection> conn = make_shared<TcpConnection>(sub_reactors_, fd, next_conn_id_);

    conn->set_connect_callback(on_connect_);
    conn->set_message_callback(on_message_);
    conn->set_close_callback([this](const shared_ptr<TcpConnection> &conn) { handle_close(conn); });
    connectionsMap_[fd] = conn;
    conn->establish_connection();

    if (active_time_ != -1.0) {
        sub_reactors_->run_after(active_time_, [this, fd] { handle_shutdown(fd); });
    }
}

void TcpServer::handle_shutdown(int fd)
{
    if (auto itr = connectionsMap_.find(fd); itr != connectionsMap_.end()) {
        LOG_DEBUG << "SERVER CLOSE CONNECTION";   
        shared_ptr<TcpConnection> conn_lock = itr->second;

        TimeStamp close_time = conn_lock->timestamp() + active_time_;
        if (close_time < TimeStamp::Now())
            conn_lock->handle_close();
        else {
            EventLoop *bind_loop = conn_lock->loop();
            bind_loop->run_at(close_time, [this, fd] { handle_shutdown(fd); });
        }
    }
}

void TcpServer::handle_close(const shared_ptr<TcpConnection> &conn)
{
    //由main_reactor_来执行`HandleCloseInLoop`函数，来保证线程安全
    //此时main_reactor_负责既负责新建连接，又负责关闭连接
    main_reactor_->run_one_func([this, conn] { handle_close_in_loop(conn); });
}

void TcpServer::handle_close_in_loop(const shared_ptr<TcpConnection> &conn)
{
    int fd = conn->fd();
    auto it = connectionsMap_.find(fd);
    assert(it != connectionsMap_.end());

    (void)it;

    connectionsMap_.erase(connectionsMap_.find(fd));
    EventLoop *loop = conn->loop();
    loop->add_one_func([conn] { conn->delete_connection(); });
}
