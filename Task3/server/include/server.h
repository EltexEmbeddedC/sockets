#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define MULTICAST_IP "239.0.0.1"
#define MESSAGE "Hi!"
#define TIMEOUT 3

void run_server();

#endif // SERVER_H
