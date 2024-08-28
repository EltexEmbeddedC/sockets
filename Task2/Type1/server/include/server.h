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
#define NEW_PORT (BASE_PORT + rand() % 10000 + 1)

extern int server_fd;

void run_server();
void* create_server(void* arg);
struct sockaddr_in get_new_endpoint();
void sigint_handler(int sig);

#endif // SERVER_H
