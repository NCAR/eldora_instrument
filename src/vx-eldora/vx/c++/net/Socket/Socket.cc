/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Socket.cc
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
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Socket.hh"

extern "C" {
#define USE_ANSI
#include "string.h"
#include "in.h"
#include "hostLib.h"
#include "ioLib.h"
};

Socket::Socket(FAST int type, FAST int family)
{
    theSocket = socket(family,type,0);

    theFamily = family;
    connected = 0;
    atomic = 0;
}

STATUS Socket::Bind(FAST int port, FAST char *host)
{
    struct sockaddr_in addr;

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;

    if (host == NULL)
      addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
      {
	  FAST int iaddr = hostGetByName(host);

	  if (!iaddr)
	    return(ERROR);
	  
	  addr.sin_addr.s_addr = htonl(iaddr);
      }

    addr.sin_port = htons(port);

    if (bind(theSocket,(struct sockaddr *)&addr,sizeof(addr)) < 0)
      return(ERROR);
    else
      return(OK);
}

STATUS Socket::Connect(FAST int port, char *host)
{
    FAST int iaddr = hostGetByName(host);

    if (!iaddr)
      return(ERROR);

    struct sockaddr_in addr;

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(iaddr);
    addr.sin_port = htons(port);

    if (connect(theSocket,(struct sockaddr *)&addr,sizeof(addr)) < 0)
      return(ERROR);
    else
      {
	  connected = 1;
	  return(OK);
      }
}

STATUS Socket::Setsockopt(int level, int optname, void *optval, int optlen)
{
    if (setsockopt(theSocket,level,optname,(char *)optval,optlen) < 0)
      return(ERROR);
    return(OK);
}

STATUS Socket::Getsockopt(int level, int optname, void *optval, int *optlen)
{
    if (getsockopt(theSocket,level,optname,(char *)optval,optlen) < 0)
      return(ERROR);
    return(OK);
}

STATUS Socket::Getpeername(void *name, int *namelen)
{
    if (getpeername(theSocket,(struct sockaddr *)name,namelen) < 0)
      return(ERROR);
    return(OK);
}

STATUS Socket::Getsockname(void *name, int *namelen)
{
    if (getsockname(theSocket,(struct sockaddr *)name,namelen) < 0)
      return(ERROR);
    return(OK);
}

STATUS Socket::Shutdown(int how)
{
    if (shutdown(theSocket,how) < 0)
      return(ERROR);
    return(OK);
}

int Socket::Write(FAST void *buffer, FAST int len)
{
    FAST char *buf = (char *)buffer;
    FAST int s = theSocket;

    if (atomic)
      {
	  FAST int count = 0;

	  while (count < len)
	    {
		FAST int i = write(s,buf,len - count);

		if (!i)
		  return(count);

		buf += i;
		count += i;
	    }
	  return(count);
      }
    else
      return(write(s,buf,len));
}

int Socket::Read(FAST void *buffer, FAST int len)
{
    FAST char *buf = (char *)buffer;
    FAST int s = theSocket;

    if (atomic)
      {
	  FAST int count = 0;

	  while (count < len)
	    {
		FAST int i = read(s,buf,len - count);

		if (!i)
		  return(count);

		buf += i;
		count += i;
	    }
	  return(count);
      }
    else
      return(read(s,buf,len));
}

int Socket::Send(FAST void *buffer, FAST int len, FAST int flags)
{
    FAST char *buf = (char *)buffer;
    FAST int s = theSocket;

    if (atomic)
      {
	  FAST int count = 0;

	  while (count < len)
	    {
		FAST int i = send(s,buf,len - count,flags);

		if (!i)
		  return(count);

		buf += i;
		count += i;
	    }
	  return(count);
      }
    else
      return(send(s,buf,len,flags));
}

int Socket::Recv(FAST void *buffer, FAST int len, FAST int flags)
{
    FAST char *buf = (char *)buffer;
    FAST int s = theSocket;

    if (atomic)
      {
	  FAST int count = 0;

	  while (count < len)
	    {
		FAST int i = recv(s,buf,len - count,flags);

		if (!i)
		  return(count);

		buf += i;
		count += i;
	    }
	  return(count);
      }
    else
      return(recv(s,buf,len,flags));
}

int Socket::Sendto(FAST void *buffer, FAST int len, FAST int flags,
		   FAST void *to, FAST int tolen)
{
    FAST char *buf = (char *)buffer;
    FAST int s = theSocket;

    if (atomic)
      {
	  FAST int count = 0;

	  while (count < len)
	    {
		FAST int i = sendto(s,buf,len - count,flags,
				    (struct sockaddr *)to,tolen);

		if (!i)
		  return(count);

		buf += i;
		count += i;
	    }
	  return(count);
      }
    else
      return(sendto(s,buf,len,flags,(struct sockaddr *)to,tolen));
}

int Socket::Recvfrom(FAST void *buffer, FAST int len, FAST int flags,
		     FAST void *from, FAST int *fromlen)
{
    FAST char *buf = (char *)buffer;
    FAST int s = theSocket;

    if (atomic)
      {
	  FAST int count = 0;

	  while (count < len)
	    {
		FAST int i = recvfrom(s,buf,len - count,flags,
				      (struct sockaddr *)from,fromlen);

		if (!i)
		  return(count);

		buf += i;
		count += i;
	    }
	  return(count);
      }
    else
      return(recvfrom(s,buf,len,flags,(struct sockaddr *)from,fromlen));
}

int Socket::Sendmsg(FAST struct msghdr *hdr, FAST int flags)
{
    return(sendmsg(theSocket,hdr,flags));
}

int Socket::Recvmsg(FAST struct msghdr *hdr, FAST int flags)
{
    return(recvmsg(theSocket,hdr,flags));
}

int Socket::Type(void)
{ 
    int i;
    int j = sizeof(int);

    if (Getsockopt(SOL_SOCKET,SO_TYPE,&i,&j) == ERROR)
      return(ERROR);

    return(i);
}
