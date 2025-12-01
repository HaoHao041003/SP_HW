#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <sys/stat.h>

/* Request (client --> server) */

struct request {
    pid_t pid;          /* PID of client */
    int seqLen;         /* Length of desired sequence */
};

/* Response (server --> client) */

struct response {
    int seqNum;         /* Start of sequence */
};

#define SERVER_KEY 0x1aaaaaa1

struct reqMSG {
    long mtype;
    struct request req;
};

struct respMSG {
    long mtype;
    struct response resp;
};