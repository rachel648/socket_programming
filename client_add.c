#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <integer1> <integer2>\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Send integers to server
    char send_buffer[BUFFER_SIZE];
    sprintf(send_buffer, "%s %s", argv[1], argv[2]);
    send(sockfd, send_buffer, strlen(send_buffer), 0);

    // Receive result from server
    int valread = read(sockfd, buffer, BUFFER_SIZE);
    printf("Sum received from server: %s\n", buffer);

    close(sockfd);
    return 0;
}