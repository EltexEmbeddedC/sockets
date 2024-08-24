#include "../include/client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void run_client() {
    int sock;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hi?";

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_port = 0;

    if (bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_sent = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bytes_sent == -1) {
        perror("sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (getsockname(sock, (struct sockaddr *)&client_addr, &client_addr_len) == -1) {
        perror("getsockname");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("I'm client\nClient started on %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    printf("Sent to server: %s\n", message);

    ssize_t bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (bytes_received == -1) {
        perror("recvfrom");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Received from server: %s\n", buffer);

    close(sock);
}
