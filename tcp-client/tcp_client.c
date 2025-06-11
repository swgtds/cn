#include <stdio.h>
#include <stdlib.h> // For exit()
#include <unistd.h> // For read(), write(), close()
#include <string.h> // For memset(), strcmp(), strlen(), strcspn()
#include <netinet/in.h> // For sockaddr_in, htons()
#include <arpa/inet.h>  // For inet_addr()
#include <sys/types.h>  // For socket, connect
#include <sys/socket.h> // For socket, connect

#define PORT 8080

int main() {
    int cli_sock_fd;
    char message[1024], buffer[1024];
    struct sockaddr_in servaddr;
    // socklen_t servaddrlen = sizeof(struct sockaddr_in); // Not strictly needed for client connect

    // Create socket file descriptor
    cli_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Page No. 07 (continuation)
    if (cli_sock_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (loopback)
    servaddr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(cli_sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while(1) {
        memset(message, 0, sizeof(message));
        printf("Enter data for server: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove trailing newline

        // Send data to server
        send(cli_sock_fd, message, strlen(message), 0);

        if (strcmp(message, "exit") == 0) {
            printf("Client exiting.\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        // Read data from server
        if (read(cli_sock_fd, buffer, sizeof(buffer) - 1) <= 0) { // Read less than full buffer
            printf("Server disconnected or read error.\n");
            break;
        }
        printf("Received data from server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Server requested exit.\n");
            break;
        }
    }

    close(cli_sock_fd); // Close the client socket
    return 0;
}