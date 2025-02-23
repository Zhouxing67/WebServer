#include "echo_server.h"
EchoServer::EchoServer(const char *ip, const int port) : server_(ip, port)
{
    server_.set_connection_callback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
    server_.set_message_callback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1));
};
EchoServer::~EchoServer() { };

void EchoServer::set_active_time(Second second)
{
    server_.set_active_time(second);
}

void EchoServer::start()
{
    server_.Start();
}

void EchoServer::onConnection(const std::shared_ptr<TcpConnection> &conn)
{
    // 获取接收连接的Ip地址和port端口
    int clnt_fd = conn->fd();
    struct sockaddr_in peeraddr;
    socklen_t peer_addrlength = sizeof(peeraddr);
    getpeername(clnt_fd, (struct sockaddr *)&peeraddr, &peer_addrlength);

    std::cout << CurrentThread::tid()
        << " EchoServer::OnNewConnection : new connection "
        << "[fd#" << clnt_fd << "]"
        << " from " << inet_ntoa(peeraddr.sin_addr) << ":" << ntohs(peeraddr.sin_port)
        << std::endl;
};

void EchoServer::onMessage(const std::shared_ptr<TcpConnection> &conn)
{
    // std::cout << CurrentThread::tid() << " EchoServer::onMessage" << std::endl;
    if (conn->state() == TcpConnection::ConnectionState::Connected) {
        std::cout << CurrentThread::tid() << " Message from clent " << conn->Rbuf_data() << std::endl;
        conn->Send(conn->Rbuf_data());
        conn->handle_close();
    }
}
