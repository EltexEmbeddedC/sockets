#include "../include/server.h"

void run_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hi!";
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("I'm server\nServer listening on port %d\n", PORT);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(address.sin_port));
    
    ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Received from client: %s\n", buffer);
    
    ssize_t bytes_sent = send(new_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("send");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Sent to client: %s\n", response);
    
    close(new_socket);
    close(server_fd);
}