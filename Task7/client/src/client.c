#include "../include/client.h"

/*
 * run_client - used to start client
 */
void run_client() {
    int fd;
    char buf[BUFFER_SIZE];
    char message[] = "Hi?";
    struct sockaddr_in addr;
    struct iphdr ip_header;
    struct udphdr udp_header;

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int one = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Client started on port %d\n", CLIENT_PORT);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    fill_udp_header(&udp_header, sizeof(message));
    fill_ip_header(&ip_header, sizeof(message));

    memcpy(buf, &ip_header, sizeof(ip_header));
    memcpy(buf + sizeof(ip_header), &udp_header, sizeof(udp_header));
    memcpy(buf + sizeof(ip_header) + sizeof(udp_header), message, sizeof(message));

    sendto(fd, buf, sizeof(ip_header) + sizeof(udp_header) + sizeof(message), 0, (struct sockaddr *)&addr, sizeof(addr));

    printf("Sent \"%s\" to server on port %d\n", message, SERVER_PORT);

    while (1) {
        ssize_t bytes_received = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received == -1) {
            perror("recvfrom");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (buf[30] == '!') {
            printf("Received from server: ");
            print_buffer(buf, (int)bytes_received, OFFSET);
            break;
        }
    }

    close(fd);
}

/*
 * fill_udp_header - Fills UDP header
 * @udp_header - pointer to the UDP struct
 * @msg_size - the length of the message
 */
void fill_udp_header(struct udphdr* udp_header, int msg_size) {
    udp_header->uh_sport = htons(CLIENT_PORT);
    udp_header->uh_dport = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(udp_header) + msg_size);
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
    ip_header->tot_len = htons(sizeof(ip_header) + sizeof(struct udphdr) + msg_size);
    ip_header->id = 0; // Packet ID (can be left as 0)
    ip_header->frag_off = 0; // No fragmentation
    ip_header->ttl = 255; // Time to live
    ip_header->protocol = IPPROTO_UDP; // Protocol (UDP)
    ip_header->check = 0; // Checksum (0 by default)
    ip_header->saddr = htonl(INADDR_LOOPBACK); // Source address
    ip_header->daddr = htonl(INADDR_LOOPBACK); // Destination address
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
