#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8090

int main() {
    int serv_sock_fd;
    char message[1024], buffer[1024];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t servaddrlen = sizeof(struct sockaddr_in);
    socklen_t cliaddrlen = sizeof(struct sockaddr_in); // Should be sizeof(cliaddr) but for sockaddr_in it's the same

    serv_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(serv_sock_fd, (struct sockaddr*)&servaddr, servaddrlen);

    while(1) {
        memset(buffer, 0, sizeof(buffer));
        recvfrom(serv_sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &cliaddrlen);
        printf("Received data from client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0)
            break;

        // Page No. 09 (continuation)
        memset(message, 0, sizeof(message));
        printf("Enter data for client: ");
        gets(message); // Note: gets() is unsafe, consider using fgets()
        sendto(serv_sock_fd, message, strlen(message), 0, (struct sockaddr*)&cliaddr, cliaddrlen);
        if (strcmp(message, "exit") == 0)
            break;
    }
    return 0;
}