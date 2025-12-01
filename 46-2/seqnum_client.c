#include "seqnum.h"

int
main(int argc, char *argv[])
{
    int serverId;
    struct reqMSG reqMsg;
    struct respMSG respMsg;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len...]\n", argv[0]);

    serverId = msgget(SERVER_KEY, S_IWUSR | S_IWGRP);
    if (serverId == -1)
        errExit("msgget - Is the server running?");


    reqMsg.mtype = 1;
    reqMsg.req.pid = getpid();
    reqMsg.req.seqLen = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;

    if (msgsnd(serverId, &reqMsg, sizeof(reqMsg), 0) == -1)
        errExit("msgsnd");


    if (msgrcv(serverId, &respMsg, sizeof(respMsg), getpid(), 0) == -1)
        errExit("msgrcv");

    printf("%d\n", respMsg.resp.seqNum);
    exit(EXIT_SUCCESS);
}