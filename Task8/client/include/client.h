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
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <ifaddrs.h>
#include <net/if.h>

#define SERVER_PORT 12345
#define CLIENT_PORT 9999
#define BUFFER_SIZE 1500
#define SERVER_IP "10.0.2.4"
#define CLIENT_IP "10.0.2.15"
#define SERVER_MAC {0x08, 0x00, 0x27, 0xa4, 0x32, 0x28}
#define CLIENT_MAC {0x08, 0x00, 0x27, 0x9f, 0x2f, 0xaf}
#define CLIENT_IF "enp0s3"
#define OFFSET 42

void run_client();
void print_buffer(char* buffer, int len, int offset);
void fill_ethernet_header(struct ethhdr* eth_header, unsigned char* src_mac, unsigned char* dest_mac);
void fill_udp_header(struct udphdr* udp_header, int msg_size);
void fill_ip_header(struct iphdr* ip_header, int msg_size);
int checksum(uint16_t* buffer, int len);

#endif // SERVER_H
