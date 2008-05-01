/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RadialDataLoop.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  91/01/02  08:52:01  thor
 * Made changes for VxWorks Version 5.0.
 * 
 * Revision 1.3  90/12/18  14:23:51  thor
 * Fixed recvfrom mistake.
 * 
 * Revision 1.2  90/12/14  10:46:17  thor
 * Working on sockets.
 * 
Revision 1.1  90/12/11  13:24:59  thor
Initial revision

 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "RadialDataLoop.hh"

#define WINDOW_GBLS
#define WINDOW_GBLS_SCOPE
#include "graphicGbl.h"

// Test case includes.
extern "C" {
#include "types.h"

#include "sockLib.h"
#include "socket.h"
#include "in.h"
#include "inetLib.h"
#include "ioLib.h"
#include "string.h"
#include "taskLib.h"
#include "stdioLib.h"
#include "errnoLib.h"
#include "usrLib.h"
};

void RadialDataLoop1(FAST Flags *flags, FAST FixedPipe *pipe)
{
    FAST DATABLOCK1 *data = (DATABLOCK1 *)malloc(sizeof(DATABLOCK1));


    for (;;)
      {
	  // This is where the data comes in!!!!
	  // May need to be massaged!!!!

	  pipe->write(data);

	  flags->set(NEW_DATA);
      }
}

void RadialDataLoop2(FAST Flags *flags, FAST FixedPipe *pipe)
{
    FAST DATABLOCK2 *data = (DATABLOCK2 *)malloc(sizeof(DATABLOCK2) * 10);
    FAST DATABLOCK2 *save = data;

taskDelay(30);
// Test case - open socket.
    FAST int sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if (sockfd < 0)
	  fprintf(stderr,"Failed to open socket\n");

    int optval = 1;

    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR,(caddr_t) &optval,
		sizeof(optval));

    struct sockaddr_in sockaddr;

    bzero((char *)&sockaddr,sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = 6000;

    if (bind(sockfd,(SOCKADDR *)&sockaddr,sizeof(sockaddr)) == ERROR)
      printErrno(errnoGet());

    FAST int index = 0;
    FAST int rollover = 9;

    for (;;)
      {
	  // This is where the data comes in!!!!
// Test case - use a socket with data preprocessed.
	  FAST int count = recvfrom(sockfd,(char *)data,sizeof(DATABLOCK2),
				    0,0,0);
	  if (!count)
	    continue;
	  else if (count == ERROR)
	    printErrno(errnoGet());

	  if (data->angle == -1.0)
	    {
		free((char *)save);
		close(sockfd);
		flags->set(DISP_QUIT);
		taskDelay(100);
	    }

	  ADDRBLOCK addr;
	  addr.angle = data->angle;
	  addr.colors = &data->colors[0];

	  pipe->write(&addr);

	  flags->set(NEW_DATA);

	  if (index == rollover)
	    {
		index = 0;
		data = save;
	    }
	  else
	    {
		index++;
		data++;
	    }
      }
}

void RadialDataLoop3(FAST Flags *flags, FAST FixedPipe *pipe)
{
    FAST DATABLOCK3 *data = (DATABLOCK3 *)malloc(sizeof(DATABLOCK3));


    for (;;)
      {
	  // This is where the data comes in!!!!
	  // May need to be massaged!!!!

	  pipe->write(data);

	  flags->set(NEW_DATA);
      }
}
