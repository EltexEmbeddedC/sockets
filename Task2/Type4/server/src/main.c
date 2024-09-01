#include "../include/server.h"

int main(int argc, char* argv[]) {
    signal(SIGINT, sigint_handler);

    if (argc != 2) {
        printf("Error: choose --select, --poll or --epoll flag\n./prog_name --flag\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp("--select", argv[1]) == 0) {
        run_server_select();
    } else if (strcmp("--poll", argv[1]) == 0) {
        run_server_poll();
    } else if (strcmp("--epoll", argv[1]) == 0) {
        run_server_epoll();
    }

    return 0;
}
