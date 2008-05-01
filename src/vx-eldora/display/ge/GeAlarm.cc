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
 * Revision 2.1  1993/08/20  17:17:03  thor
 * Dumped unused variables.
 *
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1991/10/14  19:17:48  thor
 * Eliminated unused methods & added new method for posting alarms.
 *
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

GeAlarm::GeAlarm(char *server, int sys)
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

    if (clnt == NULL)
      {
          clnt_pcreateerror(NULL);
          return;
      }
    else
      client = clnt;

    status.count = sys;		// Tell where it came from!
}

void GeAlarm::Set(int error)
{
    status.status = error;
}

GeAlarm::~GeAlarm(void)
{
    clnt_destroy(client);
}
