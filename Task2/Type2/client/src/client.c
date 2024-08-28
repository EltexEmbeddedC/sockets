#include "../include/client.h"

void run_client() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hi?";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        close(sock);
        exit(EXIT_FAILURE);
    }

    int new_port = atoi(buffer);
    printf("Received new server port: %d\n", new_port);
    close(sock);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_port = htons(new_port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Sent to new server: %s\n", message);

    bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Received from new server: %s\n", buffer);

    close(sock);
}
