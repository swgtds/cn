#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> // 
#include <netinet/in.h> 
#include <sys/types.h>  
#include <sys/socket.h> 
#include <asm-generic/socket.h>

#define PORT 8080

int main() {
    int serv_sock_fd, new_conn_fd; // serv_sock_fd for server socket, new_conn_fd for accepted client connection
    int optval = 1; // Value for setsockopt
    char message[1024], buffer[1024];
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(struct sockaddr_in);


    serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock_fd < 0) { // Changed from serv_sock_fd == 0, as socket returns -1 on error
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Page No. 05 (continuation)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(serv_sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("WAIT FOR CLIENT\n");

    // Start listening for incoming connections
    if (listen(serv_sock_fd, 3) < 0) { // 3 is the backlog queue size
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept an incoming connection
    new_conn_fd = accept(serv_sock_fd, (struct sockaddr *)&address, &addrlen);
    if (new_conn_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while(1) {
        memset(buffer, 0, sizeof(buffer));
        // Read data from client
        if (read(new_conn_fd, buffer, sizeof(buffer) - 1) <= 0) { // Read less than full buffer to null-terminate safely
            printf("Client disconnected or read error.\n");
            break;
        }
        printf("Received data from client: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested exit.\n");
            break;
        }

        memset(message, 0, sizeof(message));
        printf("Enter data for client: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove trailing newline from fgets

        // Page No. 06 (continuation)
        // Send data to client
        send(new_conn_fd, message, strlen(message), 0);

        if (strcmp(message, "exit") == 0) {
            printf("Server requested exit.\n");
            break;
        }
    }

    close(new_conn_fd); // Close the client socket
    close(serv_sock_fd); // Close the listening server socket
    return 0;
}