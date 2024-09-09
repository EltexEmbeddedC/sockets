#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SERVER_PORT 12345
#define CLIENT_PORT 9999
#define BUFFER_SIZE 64
#define OFFSET 28

typedef unsigned short u_short;

struct UdpHeader
{
    u_short src_port;
    u_short targ_port;
    u_short length;
    u_short checksum;
};

void run_server();
void print_buffer(char* buffer, int len, int offset);

#endif // SERVER_H
