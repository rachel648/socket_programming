#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <server_IP> <number1> <number2>\n", argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Get the server IP address from the command line argument
    const char* server_ip = argv[1];

    // Get the two integers from the command line arguments
    int num1 = atoi(argv[2]);
    int num2 = atoi(argv[3]);

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(1);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(1);
    }

    // Send the two integers to the server
    sprintf(buffer, "%d %d", num1, num2);
    send(sockfd, buffer, strlen(buffer), 0);

    // Receive the sum from the server
    int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("recv failed");
        exit(1);
    } else if (bytes_received == 0) {
        printf("Server disconnected\n");
        exit(1);
    }

    int sum = atoi(buffer);
    printf("Sum from server: %d\n", sum);

    close(sockfd);
    return 0;
}
