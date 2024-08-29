#include "../include/server.h"

struct socket_info* server_pool;
pthread_mutex_t pool_mutex = PTHREAD_MUTEX_INITIALIZER;

void* server_thread(void* arg) {
    struct socket_info* info = (struct socket_info*)arg;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0}, response[BUFFER_SIZE];

    sprintf(response, "Hello from server on port %d!", info->port);

    if (listen(info->server_fd, 1) < 0) {
        perror("listen");
        pthread_exit(NULL);
    }

    printf("Server listening on port %d\n", info->port);

    while (1) {
        int new_socket = accept(info->server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&pool_mutex);
        info->is_busy = 1;
        pthread_mutex_unlock(&pool_mutex);

        ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
        if (bytes_read < 0) {
            perror("read");
        } else {
            printf("Received from client on port %d: %s\n", info->port, buffer);
            if (send(new_socket, response, strlen(response), 0) == -1) {
                perror("send");
                exit(EXIT_FAILURE);
            }
            printf("Sent to client on port %d: %s\n", info->port, response);
        }

        close(new_socket);

        pthread_mutex_lock(&pool_mutex);
        info->is_busy = 0;
        pthread_mutex_unlock(&pool_mutex);
    }

    close(info->server_fd);
    pthread_exit(NULL);
}

void run_server() {
    signal(SIGINT, sigint_handler);

    server_pool = (struct socket_info*)malloc(SERVER_POOL_SIZE * sizeof(struct socket_info));
    if (server_pool == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SERVER_POOL_SIZE; i++) {
        server_pool[i].port = BASE_PORT + i + 1;
        server_pool[i].is_busy = 0;
        if ((server_pool[i].server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(server_pool[i].port);

        if (bind(server_pool[i].server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&server_pool[i].thread_id, NULL, server_thread, &server_pool[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }

        printf("Created server on port %d\n", server_pool[i].port);
    }

    printf("Main server listening on port %d\n", BASE_PORT);

    int listener_fd;
    struct sockaddr_in listener_addr;
    int addrlen = sizeof(listener_addr);

    if ((listener_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    listener_addr.sin_family = AF_INET;
    listener_addr.sin_addr.s_addr = INADDR_ANY;
    listener_addr.sin_port = htons(BASE_PORT);

    if (bind(listener_fd, (struct sockaddr*)&listener_addr, sizeof(listener_addr)) < 0) {
        perror("bind");
        close(listener_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(listener_fd, 5) < 0) {
        perror("listen");
        close(listener_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int new_socket = accept(listener_fd, (struct sockaddr*)&listener_addr, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        int found_server = 0;
        pthread_mutex_lock(&pool_mutex);
        for (int i = 0; i < SERVER_POOL_SIZE; i++) {
            if (!server_pool[i].is_busy) {
                char buffer[BUFFER_SIZE];
                sprintf(buffer, "%d", server_pool[i].port);
                if (send(new_socket, buffer, strlen(buffer), 0) == -1) {
                    perror("send");
                    close(listener_fd);
                    exit(EXIT_FAILURE);
                }
                server_pool[i].is_busy = 1;
                found_server = 1;
                break;
            }
        }
        pthread_mutex_unlock(&pool_mutex);

        if (!found_server) {
            char* message = "All servers are busy, please try again later.";
            if (send(new_socket, message, strlen(message), 0) == -1) {
                perror("send");
                close(listener_fd);
                exit(EXIT_FAILURE);
            }
        }

        close(new_socket);
    }

    close(listener_fd);
}

void sigint_handler(int sig) {
    printf("Exiting...\n");
    for (int i = 0; i < SERVER_POOL_SIZE; i++) {
        pthread_cancel(server_pool[i].thread_id);
        close(server_pool[i].server_fd);
    }
    free(server_pool);
    exit(EXIT_SUCCESS);
}
