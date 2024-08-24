#include "../include/client.h"

void run_client() {
    struct sockaddr_un serv;
    int fd;
    char buf[BUFFER_SIZE];
    char* msg = "Hi?";
    fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, SOCKET_PATH, sizeof(serv.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (send(fd, msg, strlen(msg), 0) == -1) {
        perror("send");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t n = recv(fd, buf, BUFFER_SIZE, 0);
    if (n == -1) {
        perror("recv");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buf[n] = '\0';

    printf("I'm client\nsocket path: %s\nserver will get: %s\nserver sent: %s\n", serv.sun_path, msg, buf);

    close(fd);
}