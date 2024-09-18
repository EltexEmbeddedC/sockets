#include "../include/client.h"

/*
 * run_client - used to start client
 */
void run_client() {
    int fd;
    char buf[BUFFER_SIZE];
    char message[] = "Hi?";
    struct sockaddr_ll addr = {0};
    struct ethhdr eth_header;
    struct iphdr ip_header;
    struct udphdr udp_header;
    unsigned char src_mac[6] = CLIENT_MAC, dest_mac[6] = SERVER_MAC;

    // Create RAW socket
    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Client started on interface %s\n", CLIENT_IF);

    // Init Ethernet, IP and UDP headers
    fill_ethernet_header(&eth_header, src_mac, dest_mac);
    fill_udp_header(&udp_header, sizeof(message));
    fill_ip_header(&ip_header, sizeof(message));

    // Copy Ethernet, IP and UDP headers to the buffer
    memcpy(buf, &eth_header, sizeof(eth_header));
    memcpy(buf + sizeof(eth_header), &ip_header, sizeof(ip_header));
    memcpy(buf + sizeof(eth_header) + sizeof(ip_header), &udp_header, sizeof(udp_header));
    memcpy(buf + sizeof(eth_header) + sizeof(ip_header) + sizeof(udp_header), message, sizeof(message));

    // Init sockaddr_ll struct
    addr.sll_ifindex = if_nametoindex(CLIENT_IF);  // Interface index
    addr.sll_halen = ETH_ALEN;  // MAC length
    memcpy(addr.sll_addr, dest_mac, 6);  // Destination MAC address

    sendto(fd, buf, sizeof(eth_header) + sizeof(ip_header) + sizeof(udp_header) + sizeof(message), 0, (struct sockaddr*)&addr, sizeof(addr));

    printf("Sent \"%s\" to server on port %d\n", message, SERVER_PORT);

    while (1) {
        ssize_t bytes_received = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received == -1) {
            perror("recvfrom");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (buf[OFFSET + 2] == '!') {
            printf("Received from server: ");
            print_buffer(buf, (int)bytes_received, OFFSET);
            break;
        }
    }

    close(fd);
}

/*
 * fill_ethernet_header - Fills Ethernet header
 * @eth_header - pointer to the Ethernet header structure
 * @src_mac - source MAC address
 * @dest_mac - destination MAC address
 */
void fill_ethernet_header(struct ethhdr* eth_header, unsigned char* src_mac, unsigned char* dest_mac) {
    memcpy(eth_header->h_dest, dest_mac, 6);  // Destination MAC address
    memcpy(eth_header->h_source, src_mac, 6);  // Source MAC address
    eth_header->h_proto = htons(ETH_P_IP); // EtherType for IP
}

/*
 * fill_udp_header - Fills UDP header
 * @udp_header - pointer to the UDP struct
 * @msg_size - the length of the message
 */
void fill_udp_header(struct udphdr* udp_header, int msg_size) {
    udp_header->uh_sport = htons(CLIENT_PORT);
    udp_header->uh_dport = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(struct udphdr) + msg_size);
    udp_header->check = 0;
}

/*
 * fill_ip_header - Fills IP header
 * @ip_header - pointer to the IP struct
 * @msg_size - the length of the message
 */
void fill_ip_header(struct iphdr* ip_header, int msg_size) {
    ip_header->ihl = 5; // Header length (5*4 bytes)
    ip_header->version = 4; // IPv4
    ip_header->tos = 0; // Type of service (0 by default)
    ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + msg_size);
    ip_header->id = 2; // Packet ID
    ip_header->frag_off = 0; // No fragmentation
    ip_header->ttl = 255; // Time to live
    ip_header->protocol = IPPROTO_UDP; // Protocol (UDP)
    ip_header->saddr = inet_addr(CLIENT_IP); // Source address
    ip_header->daddr = inet_addr(SERVER_IP); // Destination address
    ip_header->check = 0;
    ip_header->check = checksum((uint16_t *) ip_header, ip_header->ihl * 4);
}

int checksum(uint16_t* buffer, int len) {
    unsigned long csum = 0;

    while (len > 1) {
        csum += *buffer++;
        len -= 2;
    }

    if (len == 1) {
        csum += *(uint8_t*) buffer;
    }

    csum = (csum >> 16) + (csum & 0xFFFF);
    csum += (csum >> 16);

    return ~csum;
}

/*
 * print_buffer - used to print string starting from offset index
 * @buffer - pointer to a string
 * @len - length of the string
 * @offset - start index
 */
void print_buffer(char* buffer, int len, int offset) {
    for (int i = offset; i < len; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}
