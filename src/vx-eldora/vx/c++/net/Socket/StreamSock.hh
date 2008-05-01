/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 StreamSock.hh
 *	Original Author: Richard E. Neitzel
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
#ifndef INCStreamSockhh
#define INCStreamSockhh

#include "Socket.hh"

class StreamSock : public Socket {
  private:
    int newsock;

  public:
    StreamSock(int port, char *host = NULL);

    STATUS Accept(void);
    STATUS Accept(void *addr, int *addrlen);

    STATUS Listen(int backlog = 5);

    int Write(void *buffer, int len)
      {
	  if (!connected)
	    return(0);
	  return(Socket::Write(buffer,len));
      }

    int Read(void *buffer, int len)
      {
	  if (!connected)
	    return(0);
	  return(Socket::Read(buffer,len));
      }

    int Send(void *buffer, int len, int flags)
      {
	  if (!connected)
	    return(0);
	  return(Socket::Send(buffer,len,flags));
      }

    int Recv(void *buffer, int len, int flags)
      {
	  if (!connected)
	    return(0);
	  return(Socket::Recv(buffer,len,flags));
      }

    STATUS Linger(int lingerTime);
    
    STATUS KeepAliveOn(void)
      { return(Setsockopt(SOL_SOCKET,SO_KEEPALIVE,(char *)1,sizeof(int))); }

    STATUS KeepAliveOff(void)
      { return(Setsockopt(SOL_SOCKET,SO_KEEPALIVE,(char *)0,sizeof(int))); }

    STATUS OobInlineOn(void)
      { return(Setsockopt(SOL_SOCKET,SO_OOBINLINE,(char *)1,sizeof(int))); }

    STATUS OobInlineOff(void)
      { return(Setsockopt(SOL_SOCKET,SO_OOBINLINE,(char *)0,sizeof(int))); }

    STATUS ReuseAddrOn(void)
      { return(Setsockopt(SOL_SOCKET,SO_REUSEADDR,(char *)1,sizeof(int))); }

    STATUS ReuseAddrOff(void)
      { return(Setsockopt(SOL_SOCKET,SO_REUSEADDR,(char *)0,sizeof(int))); }
};

#endif // INCStreamSockhh

