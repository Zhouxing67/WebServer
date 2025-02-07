#include "src/EventLoop.h"
#include "src/Server.h"
#include "src/Connection.h"
#include <cstdio>


int main()
{
    EventLoop* loop = new EventLoop();
    Server *server = new Server(loop);
    server->on_connection([](Connection *conn) {
        conn->Read(false);
        if (conn->state() == Connection::State::Closed) {
            conn->Close();
            return;
        }
        const char *clnt_data = conn->Rbuf_data();
        printf("msg from %d : %s\n", conn->clnt_sockfd(), clnt_data);
        conn->set_Wbuf(clnt_data);
        conn->Write(false);
        });
        
    loop->loop();
    return 0;
}