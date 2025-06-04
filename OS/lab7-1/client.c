#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 7000
#define BUFSIZE 1024
#define IPV4 AF_INET
#define NOFLAGS 0

int set_recv_timer(int socket_fd) {
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int return_code = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (return_code == -1) {
        perror("setsockopt(SO_RCVTIMEO)");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int init_client_address(struct sockaddr_in *server_addr) {
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = IPV4;
    server_addr->sin_port = htons(PORT);

    int return_code = inet_pton(IPV4, "127.0.0.1", &server_addr->sin_addr);
    if (return_code <= 0) {
        perror("inet_pton");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void run_client(int socket_fd, struct sockaddr_in server_addr) {
    char buffer[BUFSIZE];

    printf("Type 'exit' to exit server\n");
    while (1) {
        printf("Type message: ");
        char *message = fgets(buffer, BUFSIZE, stdin);
        if (message == NULL) { break; }

        int is_exit_message = strcmp(buffer, "exit\n");
        if (is_exit_message == 0) { break; }

        ssize_t bytes_sent = sendto(socket_fd, message, strlen(message), NOFLAGS,
                                    (struct sockaddr*) &server_addr, sizeof(server_addr));
        if (bytes_sent == -1) {
            perror("sendto");
            break;
        }

        socklen_t recv_len = sizeof(server_addr);
        ssize_t bytes_recvd = recvfrom(socket_fd, buffer, BUFSIZE, NOFLAGS, (struct sockaddr*) &server_addr, &recv_len);
        if (bytes_recvd == -1) {
            perror("recvfrom");
            continue;
        }
        printf("Message from server: %s\n", buffer);
    }
}

int main(void) {
    struct sockaddr_in server_addr;
    int socket_fd = socket(IPV4, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int return_code = set_recv_timer(socket_fd);
    if (return_code == EXIT_FAILURE) {
        close(socket_fd);
        return EXIT_FAILURE;
    }

    return_code = init_client_address(&server_addr);
    if (return_code == EXIT_FAILURE) {
        close(socket_fd);
        return EXIT_FAILURE;
    }

    run_client(socket_fd, server_addr);

    close(socket_fd);
    return EXIT_SUCCESS;
}
