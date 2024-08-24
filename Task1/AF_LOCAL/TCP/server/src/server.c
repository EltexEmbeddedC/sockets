#include "../include/server.h"

void run_server() {
    struct sockaddr_un serv, client;
    socklen_t cl_len = sizeof(struct sockaddr_un);
    int fd, cl_fd, n;
    char buf[BUFFER_SIZE], buf_copy[BUFFER_SIZE];
    fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, sizeof(struct sockaddr_un));
    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, SOCKET_PATH, sizeof(serv.sun_path) - 1);

    unlink(SOCKET_PATH);
    if (bind(fd, (struct sockaddr*)&serv, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 1) == -1) {
        perror("listen");
        close(fd);
        exit(EXIT_FAILURE);
    }

    cl_fd = accept(fd, (struct sockaddr*)&client, &cl_len);
    if (cl_fd == -1) {
        perror("accept");
        close(fd);
        exit(EXIT_FAILURE);
    }

    n = recv(cl_fd, buf, BUFFER_SIZE, 0);
    if (n == -1) {
        perror("recv");
        close(fd);
        close(cl_fd);
        exit(EXIT_FAILURE);
    }

    strncpy(buf_copy, buf, BUFFER_SIZE);

    if (n == 3) {
        buf[2] = '!';
        buf[3] = '\0';
    }

    if (send(cl_fd, buf, strlen(buf), 0) == -1) {
        perror("send");
        close(fd);
        close(cl_fd);
        exit(EXIT_FAILURE);
    }

    printf("I'm server\nsocket path: %s\nclient sent: %s\nclient will get: %s\n", serv.sun_path, buf_copy, buf);

    close(fd);
    close(cl_fd);
    unlink(SOCKET_PATH);
}