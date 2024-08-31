#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <stdlib.h>

bool stop = false;

void handle_sigint(const int sig) {
    printf("Caught signal %d (Ctrl+C). Exiting gracefully...\n", sig);
    stop = true;
}

int main(void) {
    signal(SIGINT, handle_sigint);

    //  Socket to talk to clients
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    assert(rc == 0);

    while (!stop) {
        char buffer[10];
        zmq_recv(responder, buffer, 10, 0);
        printf("Received Hello\n");
        sleep(1); //  Do some 'work'
        zmq_send(responder, "World", 5, 0);
    }

    return 0;
}
