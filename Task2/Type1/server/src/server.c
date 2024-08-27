#include "../include/server.h"

void* create_server(void* arg) {
    int new_port = *((int*)arg);
    free(arg);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0}, response[BUFFER_SIZE];
    sprintf(response, "Hello from new server! (%d)", new_port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        pthread_exit(NULL);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(new_port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        pthread_exit(NULL);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        pthread_exit(NULL);
    }

    printf("New server listening on port %d\n", new_port);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        pthread_exit(NULL);
    }

    ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        close(new_socket);
        close(server_fd);
        pthread_exit(NULL);
    }
    printf("Received from client: %s\n", buffer);

    ssize_t bytes_sent = send(new_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("send");
        close(new_socket);
        close(server_fd);
        pthread_exit(NULL);
    }
    printf("Sent to client: %s\n", response);

    close(new_socket);
    close(server_fd);
    pthread_exit(NULL);
}

void run_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(BASE_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("I'm server\nServer listening on port %d\n", BASE_PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        int new_port = NEW_PORT;
        printf("Creating new server on port %d\n", new_port);

        int* port_ptr = malloc(sizeof(int));
        if (port_ptr == NULL) {
            perror("malloc");
            close(server_fd);
            close(new_socket);
            exit(EXIT_FAILURE);
        }
        *port_ptr = new_port;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, create_server, port_ptr) != 0) {
            perror("pthread_create");
            close(server_fd);
            close(new_socket);
            exit(EXIT_FAILURE);
        }

        sprintf(buffer, "%d", new_port);
        if (send(new_socket, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            close(server_fd);
            close(new_socket);
            exit(EXIT_FAILURE);
        }

        close(new_socket);
    }

    close(server_fd);
}
