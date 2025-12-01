#include <signal.h>
#include "seqnum.h"

int
main(int argc, char *argv[])
{
    int serverId;
    struct reqMSG reqMsg;
    struct respMSG respMsg;
    int seqNum = 0;             /* This is our "service" */

    serverId = msgget(SERVER_KEY, IPC_CREAT | S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH);
    if (serverId == -1)
        errExit("msgget");

    for (;;) {                  /* Read requests and send responses */
        if (msgrcv(serverId, &reqMsg, sizeof(reqMsg), 1, 0) == -1)
        {
            if (errno == EINTR)
                continue;
            errMsg("msgrcv");
            break;
        }

        respMsg.mtype = reqMsg.req.pid;
        respMsg.resp.seqNum = seqNum;

        if (msgsnd(serverId, &respMsg, sizeof(respMsg), 0) == -1)
        {
            errMsg("msgsnd");
        }

        seqNum += reqMsg.req.seqLen;
    }
    
    if (msgctl(serverId, IPC_RMID, NULL) == -1)
        errExit("msgctl");

    exit(EXIT_SUCCESS);
}