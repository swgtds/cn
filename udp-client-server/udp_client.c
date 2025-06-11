#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> // <arpa/inet.h> is usually needed for inet_addr but often included by netinet/in.h

#define PORT 8090

int main() {
    int cli_sock_fd;
    char message[1024], buffer[1024];
    struct sockaddr_in servaddr;
    socklen_t servaddrlen = sizeof(struct sockaddr_in);

    cli_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Loopback address
    servaddr.sin_port = htons(PORT);

    while(1) {
        memset(message, 0, sizeof(message));
        // Page No. 10 (continuation)
        printf("Enter data for server: ");
        gets(message); // Note: gets() is unsafe, consider using fgets()
        sendto(cli_sock_fd, message, strlen(message), 0, (struct sockaddr*)&servaddr, servaddrlen);
        if (strcmp(message, "exit") == 0)
            break;

        memset(buffer, 0, sizeof(buffer));
        recvfrom(cli_sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &servaddrlen); // &servaddrlen is correct
        printf("Received data from server: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0)
            break;
    }
    return 0;
}