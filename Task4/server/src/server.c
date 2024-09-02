#include "../include/server.h"

void run_server() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    int broadcast_permission = 1;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_permission, sizeof(broadcast_permission)) < 0) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    while (1) {
        if (sendto(sockfd, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
            perror("sendto");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Sent broadcast: %s\n", MESSAGE);
        sleep(TIMEOUT);
    }

    close(sockfd);
}
