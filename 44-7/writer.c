#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int fd;
    int flags;
    char buffer[256];

    flags = O_WRONLY;
    if (argc > 1 && strcmp(argv[1], "-n") == 0)
    {
        flags |= O_NONBLOCK;
        printf("[Writer] Opening FIFO in NON-BLOCKING mode...\n");
    }
    else 
    {
        printf("[Writer] Opening FIFO in BLOCKING mode (waiting for reader)...\n");
    }

    fd = open("f1", flags);
    if (fd == -1)
    {
        if (errno == ENXIO)
        {
            fprintf(stderr, "[Writer] Error: No reader connected (ENXIO).\n");
            fprintf(stderr, "         In non-blocking mode, open() fails if no reader exists.\n");
        }
        else
        {
            perror("[Writer] open");
        }
        exit(EXIT_FAILURE);
    }
    printf("[Writer] FIFO opened successfully.\n");

    printf("[Writer] Enter text to send (Ctrl+D to finish):\n");
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (write(fd, buffer, strlen(buffer)) == -1)
        {
            if (errno == EAGAIN)
            {
                printf("[Writer] Write failed (EAGAIN) - Pipe full?\n");
            }
            else if (errno == EPIPE)
            {
                printf("[Writer] Broken pipe (Reader disconnected).\n");
                break;
            }
            else
            {
                perror("[Writer] write");
                break;
            }
        }
    }

    close(fd);
    printf("[Writer] Closed FIFO.\n");
    return 0;
}