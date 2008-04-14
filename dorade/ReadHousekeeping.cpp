//
// Read housekeeping packets.
//
# include <cerrno>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <cstring>
# include <netdb.h>
# include <unistd.h>
# include <netinet/in.h>
# include <signal.h>
# include <syslog.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>

# include "DoradeDescriptor.h"
# include "DoradeASIB.h"
# include "DoradeRYIB.h"
# include "DoradeFRAD.h"

void makeAddr(struct sockaddr_in *addr, const char* hostname, int port);
int getPort(const char* portname);
int openInSocket(const struct sockaddr_in* inAddr);
void unpackHousekeeping(const unsigned char* buf, int buflen);


int
main(int argc, char *argv[])
{
    int srcPort = 2222;
    //
    // Open the socket
    //
    struct sockaddr_in srcAddr;
    makeAddr(&srcAddr, 0, srcPort);
    int inSocket = openInSocket(&srcAddr);
    //
    // Now just wait for packets
    //
    unsigned char buf[4096];

    while (1)
    {
        int nread;
        struct sockaddr_in sender;
        int fromlen = sizeof(sender);
        if ((nread = recvfrom(inSocket, buf, sizeof(buf) - 1, 0, 
                              (struct sockaddr*)&sender, 
                              (socklen_t*)&fromlen)) < 0)
        {
            perror("reading from incoming socket");
            exit(1);
        }
        if (nread != 176) {
            std::cerr << "Got " << nread << 
                "bytes instead of the expected 176" << std::endl;
            continue;
        }
        unpackHousekeeping(buf, nread);
   }
}


void
unpackHousekeeping(const unsigned char* buf, int buflen) {
    const unsigned char* data = buf;
    int datalen = buflen;
    
    DoradeRYIB *ryib = 0;
    DoradeASIB *asib = 0;
    DoradeFRAD *frad = 0;

    try {
        ryib = new DoradeRYIB(data, datalen, false);
        data += ryib->getDescLen();
        datalen -= ryib->getDescLen();
        
        asib = new DoradeASIB(data, datalen, false);
        data += asib->getDescLen();
        datalen -= asib->getDescLen();
        
        frad = new DoradeFRAD(data, datalen, false);
    } catch (DescriptorException dex) {
        std::cerr << std::string("Descriptor exception: ") + dex.what() << 
            std::endl;
        return;
    }
    
    std::cout << frad->getRadarName() << " " << ryib->getRayDateTime() << 
        std::endl;
}

int 
openInSocket(const struct sockaddr_in* inAddr)
//
// Open the incoming socket
//
{
    //
    // Create the UDP socket
    //
    int inSocket;
    int on = 1;

    if ((inSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
            setsockopt(inSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 ||
            bind(inSocket, (struct sockaddr*)inAddr, sizeof(*inAddr)) < 0)
    {
        perror("creating incoming UDP socket");
        exit(1);
    }

    return inSocket;
}



void
makeAddr(struct sockaddr_in *addr, const char* hostname, int port)
//
// Fill in a struct sockaddr_in, using the given host name and port.
// If the hostname is null, the host address INADDR_ANY is used.
//
{
    memset(addr, 0, sizeof(addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (hostname)
    {
        struct hostent* hent = gethostbyname(hostname);
        if (! hent)
        {
            fprintf(stderr, "Lookup of host '%s' failed\n",
                    hostname);
            exit(1);
        }
        addr->sin_addr.s_addr = *(long*)hent->h_addr;
    }
    else
        addr->sin_addr.s_addr = INADDR_ANY;
}


int
getPort(const char* portname)
//
// Try to convert the given string to a (local order) port number, first
// trying for a named service entry, then trying to parse the string as an
// integer port number.
//
{
    struct servent* entry;

    if ((entry = getservbyname(portname, "udp")) != 0)
        return(ntohs(entry->s_port));

    char *endptr;

    int portnum = strtol(portname, &endptr, 0);
    if (endptr == portname)
    {
        fprintf(stderr, "Bad port name/number '%s'\n", portname);
        exit(1);
    }
    return portnum;
}
