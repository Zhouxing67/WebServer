#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;
sockaddr_in serv_addr, clnt_addr;
char buffer[BUF_SIZE];
//bzero(&serv_addr, sizeof(serv_addr));
int main()
{
    bzero(&serv_addr, sizeof(serv_addr));
    bzero(&clnt_addr, sizeof(sockaddr));
    memset(buffer, 0, BUF_SIZE);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(sockaddr_in));

    while (true) {
        listen(sockfd, SOMAXCONN);
        socklen_t cli_sock_addr_len = sizeof(clnt_addr);
        int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &cli_sock_addr_len);
        printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    }

}
