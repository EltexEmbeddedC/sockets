#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <malloc.h>
#include <errno.h>

#define PORT_TCP 12345
#define PORT_UDP 12346
#define BUFFER_SIZE 1024

void run_server_select();
void run_server_poll();
void run_server_epoll();

void process_tcp(int server_fd, struct sockaddr_in address);
void process_udp(int server_fd);
void init_tcp_fd(int* server_fd_tcp, struct sockaddr_in* address_tcp);
void init_udp_fd(int* server_fd_udp, struct sockaddr_in* address_udp);
void sigint_handler(int sig);

#endif // SERVER_H
