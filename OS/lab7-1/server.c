#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000
#define BUFSIZE 1024
#define IPV4 AF_INET
#define NOFLAGS 0

int init_server(int socket_fd) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = IPV4;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    int return_code = bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (return_code < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void run_server(int socket_fd) {
    struct sockaddr_in client_addr;
    char buffer[BUFSIZE];
    while (1) {
        socklen_t recv_len = sizeof(client_addr);
        ssize_t bytes_recvd = recvfrom(socket_fd, buffer, BUFSIZE, NOFLAGS, (struct sockaddr*)&client_addr, &recv_len);
        if (bytes_recvd < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[bytes_recvd] = '\0';

        char* client_ip = inet_ntoa(client_addr.sin_addr);
        if (client_ip == NULL) {
            perror("inet_ntoa");
            continue;
        }
        int client_port = ntohs(client_addr.sin_port);
        printf("Received from %s:%d - %s\n", client_ip, client_port, buffer);

        ssize_t bytes_sent = sendto(socket_fd, buffer, bytes_recvd, NOFLAGS, (struct sockaddr*)&client_addr, recv_len);
        if (bytes_sent < 0) {
            perror("sendto");
            continue;
        }
        printf("Sent to %s:%d - %s\n", client_ip, client_port, buffer);
    }
}

int main(void) {
    int socket_fd = socket(IPV4, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int return_code = init_server(socket_fd);
    if (return_code == EXIT_FAILURE) {
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("UDP echo server is running on port %d\n", PORT);
    run_server(socket_fd);

    close(socket_fd);
    return EXIT_SUCCESS;
}
