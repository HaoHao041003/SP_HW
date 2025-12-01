#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
//#include <signal.h>

int main(int argc, char *argv[])
{
    char buffer[256];
    int fd, flags;
    ssize_t numRead;
    umask(0);
    if (mkfifo ("f1", S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    flags = O_RDONLY;
    if (argc > 1 && strcmp(argv[1], "-n") == 0)
    {
        flags |= O_NONBLOCK;
        printf("[Reader] Opening FIFO in NON-BLOCKING mode...\n");
    }
    else
    {
        printf("[Reader] Opening FIFO in BLOCKING mode (waiting for writer)...\n");
    }

    fd = open("f1", flags);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        numRead = read(fd, buffer, sizeof(buffer));
        
        if (numRead == -1)
        {
            if (errno == EAGAIN)
            {
                printf("[Reader] Non-blocking read: No data available (EAGAIN). Retrying...\n");
                sleep(1);
                continue;
            }
            else
            {
                perror("[Reader] read");
                break;
            }
        }
        else if (numRead == 0)
        {
            printf("[Reader] End of File (Writer closed).\n");
            break;
        }

        buffer[numRead] = '\0';
        printf("[Reader] Read %ld bytes: %s", (long)numRead, buffer);
        sleep(1);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}