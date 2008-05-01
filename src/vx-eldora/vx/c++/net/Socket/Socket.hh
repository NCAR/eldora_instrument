/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Socket.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCSockethh
#define INCSockethh

extern "C" {
#include "vxWorks.h"
#include "socket.h"
#include "sockLib.h"
};

class Socket {
  protected:
    int theSocket;
    int connected;
    int theFamily;
    int atomic;

  public:
    Socket(int type, int family = AF_INET);
    
    int Getsockd(void) { return(theSocket); }
    void SetAtomicOn(void) { atomic++; }
    void SetAtomicOff(void) { atomic = 0; }

    STATUS Bind(int port, char *host = NULL);

    STATUS Connect(int port, char *host);

    int Write(void *buffer, int len);

    int Send(void *buffer, int len, int flags);
    int Sendto(void *buffer, int len, int flags, void *to, int tolen);
    int Sendmsg(struct msghdr *hdr, int flags);

    int Read(void *buffer, int len);

    int Recv(void *buffer, int len, int flags);
    int Recvfrom(void *buffer, int len, int flags, void *from, int *fromlen);
    int Recvmsg(struct msghdr *hdr, int flags);

    STATUS Setsockopt(int level, int optname, void *optval, int optlen);

    STATUS Getsockopt(int level, int optname, void *optval, int *optlen);

    STATUS Getpeername(void *name, int *namelen);

    STATUS Getsockname(void *name, int *namelen);

    STATUS Shutdown(int how);
    
    STATUS SetRecvSize(int size)
      { return(Setsockopt(SOL_SOCKET,SO_SNDBUF,(char *)&size,sizeof(int))); }

    STATUS GetRecvSize(int size)
      {
	  int i = sizeof(int);
	  return(Getsockopt(SOL_SOCKET,SO_SNDBUF,(char *)&size,&i));
      }

    STATUS SetSendSize(int size)
      { return(Setsockopt(SOL_SOCKET,SO_RCVBUF,(char *)&size,sizeof(int))); }

    STATUS GetSendSize(int size)
      {
	  int i = sizeof(int);
	  return(Getsockopt(SOL_SOCKET,SO_RCVBUF,(char *)&size,&i));
      }

    STATUS DontrouteOn(void)
      { return(Setsockopt(SOL_SOCKET,SO_DONTROUTE,(char *)1,sizeof(int))); }
	  
    STATUS DontrouteOff(void)
      { return(Setsockopt(SOL_SOCKET,SO_DONTROUTE,(char *)0,sizeof(int))); }

    int Type(void);
};

#endif // INCSockethh

