#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define BASE_PORT 7777
#define BUFFER_SIZE 1024
#define SERVER_POOL_SIZE 5

struct socket_info {
    int server_fd;
    int port;
    int is_busy;
    pthread_t thread_id;
};

extern struct socket_info* server_pool;
extern pthread_mutex_t pool_mutex;

void run_server();
void* server_thread(void* arg);
void sigint_handler(int sig);

#endif // SERVER_H
