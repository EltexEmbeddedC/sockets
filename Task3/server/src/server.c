#include "../include/server.h"

void run_server() {
    int sockfd;
    struct sockaddr_in multicast_addr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(PORT);
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);

    while (1) {
        if (sendto(sockfd, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr)) < 0) {
            perror("sendto");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Sent multicast: %s\n", MESSAGE);
        sleep(TIMEOUT);
    }

    close(sockfd);
}
