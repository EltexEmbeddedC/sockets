#include "../include/client.h"

void run_client() {
    struct sockaddr_un serv, client;
    int fd;
    char buf[BUFFER_SIZE];
    char* msg = "Hi?";

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, SOCKET_PATH_SERVER, sizeof(serv.sun_path) - 1);

    memset(&client, 0, sizeof(struct sockaddr_un));
    client.sun_family = AF_LOCAL;
    strncpy(client.sun_path, SOCKET_PATH_CLIENT, sizeof(client.sun_path) - 1);

    unlink(SOCKET_PATH_CLIENT);
    if (bind(fd, (struct sockaddr*)&client, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&serv, sizeof(serv)) == -1) {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t n = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, NULL);
    if (n == -1) {
        perror("recvfrom");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buf[n] = '\0';

    printf("I'm client\nsocket server path: %s\nsocket client path: %s\nserver will get: %s\nserver sent: %s\n", serv.sun_path, client.sun_path, msg, buf);

    close(fd);
    unlink(SOCKET_PATH_CLIENT);
}
