#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define SERVER_PORT 12345
#define CLIENT_PORT 9999
#define BUFFER_SIZE 64
#define OFFSET 28

void run_client();
void print_buffer(char* buffer, int len, int offset);
void fill_udp_header(struct udphdr* udp_header, int msg_size);
void fill_ip_header(struct iphdr* ip_header, int msg_size);

#endif // SERVER_H
