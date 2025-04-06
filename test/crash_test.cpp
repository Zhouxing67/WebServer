#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
int main()
{
    int servfd;
    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(EXIT_FAILURE);
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(10000);

    if (bind(servfd, (sockaddr *)&addr, sizeof(addr)) == -1)
        exit(EXIT_FAILURE);

    if (listen(servfd, 4096) == -1)
        exit(EXIT_FAILURE);

    socklen_t len;
    struct sockaddr_in clntaddr;
    if (accept(servfd, (sockaddr *)&clntaddr, &len))
        exit(EXIT_FAILURE);

    while (1)
        ;
}