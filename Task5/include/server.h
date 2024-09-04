#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define OFFSET 28

void run_server();
void print_buffer(char* buffer, int len, int offset);

#endif // SERVER_H
