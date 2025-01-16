#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H
#include <arpa/inet.h>

class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};

#endif

