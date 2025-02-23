#include "echo_server.h"

int main(int argc, char *argv[])
{
    EchoServer server("127.0.0.1", 8888);
    server.set_active_time(10.0);
    server.start();
    return 0;
}

