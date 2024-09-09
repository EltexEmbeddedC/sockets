#include "../include/client.h"

/*
 * run_server - used to start sniffer 
 */
void run_server() {
    int fd;
    char buf[BUFFER_SIZE];
    char message[] = "Hi?";
    struct sockaddr_in addr;
    struct UdpHeader header;


    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Client started on port %d\n", CLIENT_PORT);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    header.src_port = htons(CLIENT_PORT);
    header.targ_port = htons(SERVER_PORT);
    header.length = htons(sizeof(header)+sizeof(message));
    header.checksum = 0;
    
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
