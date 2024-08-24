#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH_SERVER "/tmp/stream_server"
#define SOCKET_PATH_CLIENT "/tmp/stream_client"
#define BUFFER_SIZE 1024

void run_client();

#endif // CLIENT_H
