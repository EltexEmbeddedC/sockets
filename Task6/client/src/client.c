#include "../include/client.h"

/*
 * run_client - used to start client
 */
void run_client() {
    int fd;
    char buf[BUFFER_SIZE];
    char message[] = "Hi?";
    struct sockaddr_in addr;
    struct udphdr header;


    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Client started on port %d\n", CLIENT_PORT);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    fill_udp_header(&header, sizeof(message));
    
    memcpy((void *)buf, (void *)&header, sizeof(header));
    memcpy((void *)(buf+sizeof(header)), (void *)message, sizeof(message));

    sendto(fd, buf, sizeof(header)+sizeof(message), 0, (struct sockaddr *)&addr, sizeof(addr));

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
