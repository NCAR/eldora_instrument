/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 StreamSock.cc
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

#include "StreamSock.hh"

StreamSock::StreamSock(FAST int port, char *host) : Socket(SOCK_STREAM)
{
    if (Getsockd() < 0)
      return;

    if (Bind(port) != OK)
      return;

    if (host != NULL)
      Connect(port,host);
}

STATUS StreamSock::Accept(void)
{
    int i;
    sockaddr addr;

    return(Accept(&addr,&i));
}

STATUS StreamSock::Accept(void *addr, int *addrlen)
{
    if ((newsock = accept(theSocket,(struct sockaddr *)addr,addrlen)) < 0)
      return(ERROR);
    return(OK);
}

STATUS StreamSock::Listen(FAST int backlog)
{
    if (backlog > 5)
      backlog = 5;

    if (listen(theSocket,backlog) < 0)
      return(ERROR);
    return(OK);
}

STATUS StreamSock::Linger(int lingerTime)
{
    struct linger opt;

    if (!lingerTime)
      opt.l_onoff = 0;
    else
      {
	  opt.l_onoff = 1;
	  opt.l_linger = lingerTime;
      }

    return(Setsockopt(SOL_SOCKET,SO_LINGER,&opt,sizeof(opt)));
}

    
