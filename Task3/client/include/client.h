#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define MULTICAST_IP "239.0.0.1"
#define BUFFER_SIZE 1024

struct ip_mreq  {
	struct in_addr imr_multiaddr;	/* IP multicast address of group */
	struct in_addr imr_interface;	/* local IP address of interface */
};

void run_client();

#endif // CLIENT_H
