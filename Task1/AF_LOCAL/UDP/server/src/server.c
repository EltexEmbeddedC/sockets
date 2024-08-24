#include "../include/server.h"

void run_server() {
    struct sockaddr_un serv, client;
    socklen_t cl_len = sizeof(struct sockaddr_un);
    int fd, n;
    char buf[BUFFER_SIZE], buf_copy[BUFFER_SIZE];

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, sizeof(struct sockaddr_un));
    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, SOCKET_PATH_SERVER, sizeof(serv.sun_path) - 1);

    unlink(SOCKET_PATH_SERVER);
    if (bind(fd, (struct sockaddr*)&serv, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    n = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct sockaddr*)&client, &cl_len);
    if (n == -1) {
        perror("recvfrom");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (n == 3) {
        buf[3] = '\0';
    }

    strcpy(buf_copy, buf);

    buf[2] = '!';


    if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&client, cl_len) == -1) {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("I'm server\nsocket server path: %s\nsocket client path: %s\nclient sent: %s\nclient will get: %s\n", serv.sun_path, client.sun_path, buf_copy, buf);

    close(fd);
    unlink(SOCKET_PATH_SERVER);
}
