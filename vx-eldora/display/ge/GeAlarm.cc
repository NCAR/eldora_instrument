/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeAlarm.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/10/01  16:15:14  thor
 * Initial revision
 *
 *
 *
 * description:
 *        Methods for GeAlarm class.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeAlarm.hh"

extern "C" {
#include "hostLib.h"
#include "rpcLib.h"
#include "socket.h"
#include "taskLib.h"
#include "ioLib.h"
};

GeAlarm::GeAlarm(char *server, int system)
{
    struct timeval tmo = { 10, 0 };

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = (u_long)hostGetByName(server);

    int s = RPC_ANYSOCK;

    rpcTaskInit();

    FAST int j = 10;

    FAST CLIENT *clnt;

    for (FAST int i = 0; i < j; i++) // Repeat up to 10 times before failing.
      {
	  if ((clnt = clntudp_create(&addr,DispAlarm,DispAlarmVers,tmo,
				       &s)) != NULL)
	    break;

	  taskDelay(60);
      }

    socket = s;

    if (clnt == NULL)
      {
          clnt_pcreateerror(NULL);
          return;
      }
    else
      client = clnt;

    clnt_control(clnt,CLSET_FD_CLOSE,NULL); // May be someday WRS will
					    // have clnt_destroy.

    status.count = system;	// Tell where it came from!
}

void GeAlarm::Set(int error)
{
    status.status = error;
}

GeAlarm::~GeAlarm(void)
{
    free((char *)client);	// These really should be done with
				// clnt_destroy. 
    close(socket);
}