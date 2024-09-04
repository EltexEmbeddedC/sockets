#include "../include/server.h"

/*
 * run_server - used to start sniffer 
 */
void run_server() {
    int fd;
    char buf[BUFFER_SIZE];

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t bytes_received = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received == -1) {
            perror("recvfrom");
            close(fd);
            exit(EXIT_FAILURE);
        }
        
        if (buf[OFFSET] == 'H') {
            print_buffer(buf, (int)bytes_received, OFFSET);
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
