// read then write
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char * myfifo = "fifo";
    char message[1024];
    char buffer[1024];
    mkfifo(myfifo, 0666);
    while (1)
    {
        fd = open(myfifo, O_RDONLY);
        memset(buffer, 0, sizeof(buffer));
        read(fd, buffer, 1024);
        printf("Receive data from 1st program: %s\n", buffer);
        close(fd);
        if(strcmp(buffer, "exit")==0)
            break;
        fd = open(myfifo, O_WRONLY );
        printf("Enter Data for 1st program:");
        gets(message);
        write(fd, message, 1024);
        close(fd);
        if(strcmp(message, "exit")==0)
            break;
        
    }
    return 0;
}
