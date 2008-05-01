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



extern "C" {
    void
    SendUDP(char* outstring)
    {
        
        //printf("Sending UDP Packet Now.\n");
    
        char *destHost = "drx";
        int destPort = 2222;
        struct sockaddr_in destAddr;

        makeAddr(&destAddr, destHost, destPort);
       
        //
        // Now just write the string to the destination
        //
            int nwrote;
            if ((nwrote = sendto(UDPSocket, outstring, sizeof(struct DATARAY), 0, 
                                 (struct sockaddr*)&destAddr, 
                                 sizeof(destAddr))) < 0)
            {
                char message[256];
                sprintf(message, "writing '%s' to %s:%d", outstring, destHost,
                        destPort);
                perror(message);
            }
           // printf("Wrote %i bytes\n", nwrote);

        return;       

    }
} /* end extern "C" */