# listen
### 函数原型
#include＜sys/socket.h＞
int listen(int sockfd,int backlog);
### 功能
sockfd参数指定被监听的socket。backlog参数提示内核监听队列的最大长度。
监听队列的长度如果超过backlog，服务器将不受理新的客户连接，客户端也将收到ECONNREFUSED错误信息。在内核版本2.2之前的Linux中，backlog参数是指所有处于半连接状态SYN_RCVD）和完全连接状态（ESTABLISHED）的socket的上限。但自内核版本2.2之后，它只表示处于完全连接状态的socket的上限，处于半连接状态的socket的上限则由/proc/sys/net/ipv4/tcp_max_syn_backlog内核参数定义。
backlog参数的典型值是5。

# 实用小工具telnet netstat 
netstat 是一个网络相关的命令行工具，用于显示网络连接、路由表、接口统计信息以及网络协议的相关信息。它是 network statistics 的缩写，常用于网络故障排除和网络监控。
telnet 是一种用于远程连接的协议和命令行工具，通常用于通过网络连接到远程设备的指定端口。它最常用于测试和诊断网络服务的连接情况，尤其是通过 TCP/IP 连接特定端口（例如，测试一个服务器的特定端口是否开放）。