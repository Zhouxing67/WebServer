#include "echo_server.h"

int main(int argc, char *argv[])
{
    EchoServer server("127.0.0.1", 8888);
    server.start();
    return 0;
}

