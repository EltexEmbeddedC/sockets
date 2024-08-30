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
#include <sys/ipc.h>
#include <sys/msg.h>

#define BASE_PORT 7777
#define BUFFER_SIZE 1024
#define SERVER_POOL_SIZE 5
#define QUEUE_KEY 1234

struct socket_info {
    int port;
    pthread_t thread_id;
};

struct msg_buffer {
    long msg_type;
    int client_fd;
};

extern struct socket_info* server_pool;
extern pthread_mutex_t pool_mutex;

void run_server();
void* server_thread(void* arg);
void sigint_handler(int sig);
void handle_client_request(struct socket_info* info, int client_fd, char* buffer, char* response);
void listener_main_loop(int listener_fd, int msgid);
void enqueue_client_request(int client_fd, int msgid);
int init_listener_socket();
void init_server_pool();

#endif // SERVER_H
