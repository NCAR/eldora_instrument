# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>

/* vxWorks headers */
# include <sockLib.h>
# include <sysLib.h>
# include <taskLib.h>
# include <hostLib.h>
/* end vxWorks headers */

#define scope extern
#include "hskpAll.h"



void makeAddr(struct sockaddr_in *addr, char* host, int port);
int openOutSocket(void);

extern "C" {
    void
    init_UDP()
    {
        char *destHost = "drx";
        int destPort = 2222;
        struct sockaddr_in destAddr;

        makeAddr(&destAddr, destHost, destPort);
        //
        // Open the socket
        //
        UDPSocket = openOutSocket();
        
        return;       

    }
} /* end extern "C" */



int 
openOutSocket(void) 
//
// Open the outgoing socket
//
{
    //
    // Create the UDP socket
    //
    int outSocket;

    if ((outSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("creating outgoing UDP socket");
        exit(1);
    }

    return outSocket;
}



void
makeAddr(struct sockaddr_in *addr, char* host, int port)
//
// Fill in a struct sockaddr_in, using the given host string (in dot notation) 
// and port number.
//
{
    memset(addr, 0, sizeof(addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    /* addr->sin_addr.s_addr = inet_addr(host); */
    addr->sin_addr.s_addr = hostGetByName(host);
}
