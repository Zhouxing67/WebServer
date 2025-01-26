#ifndef SOCKET_H
#define SOCKET_H

class InetAddress;
class Socket
{
private:
    int fd_ = -1;
public:
    Socket();
    Socket(int);
    ~Socket();

    int getFd() { return fd_; }

    void bind(InetAddress*);
    void listen();
    void setnonblocking();
    Socket *accept(InetAddress *);
    void connect(InetAddress *);
};
#endif

