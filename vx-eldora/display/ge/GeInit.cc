/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeInit.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.6  1992/10/02  20:43:24  thor
 * Changed stack sizes of tasks.
 *
 * Revision 1.5  1992/01/28  16:00:46  thor
 * Added tp41 specific hardware initialization.
 *
 * Revision 1.4  1991/11/01  20:02:24  thor
 * Added new static for time keeping.
 *
 * Revision 1.3  1991/10/17  16:48:44  thor
 * Increased stack size.
 *
 * Revision 1.2  1991/10/14  19:21:29  thor
 * Changed to use Task class & added new tasks to spawn.
 *
 * Revision 1.1  1991/10/01  16:15:09  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define GE_SCOPE
#include "GeGlobal.hh"

extern "C" {
#include "stdioLib.h"
#include "rpcLib.h"
#include "tp41Lib.h"
#include "sysLib.h"
};

static DispStatus ge_status;
static DispCommand ge_command;
static Beam_Time lasttime;

void GeStart(char *server, int sys)
{
    // Put hardware initialization, etc. here.

    sysIntEnable(1);		// Enable interrupts.
    sysIntEnable(2);
    sysIntEnable(3);

    dport((char *)0x40200000,(void *)0x10000000,4); // Set up dual
						    // ported memory.

    wcio(0,"a",0xc8);		// Set VME page & attach VME ext space.

    system = sys;

    Alarm = NULL;

    GeStatus = &ge_status;
    GeCommand = &ge_command;
    LastTime = &lasttime;

    ge_status.status = LOADED;
    ge_status.count = 0;

    // Start graphics task.
    DrawingTask = new Task((FUNCPTR)DrawingLoop,NULL,0,DRAWING_PRI,18000);

    // Start alarm task.

    int args[2];

    args[0] = (int)server;
    args[1] = sys;

    AlarmTask = new Task((FUNCPTR)AlarmLoop,args,2,ALARM_PRI,18000); 

    // Now we start control loop.
    CtrlTask = new Task((FUNCPTR)RpcLoop,NULL,0,CTRL_PRI,18000);
}

void RpcLoop(Task &self)
{
    if (rpcTaskInit() == ERROR)
      {
	  fprintf(stderr,"Failed to initialize Rpc.");
	  return;
      }

    if (DispRpcInit() < 0)
      return;

    startHeader();

    svc_run();
}

void AlarmLoop(FAST Task &self, FAST char *server, FAST int sys)
{
    self.FlagsInit();

    self.WaitOnFlags(ATASK_RESET,FLAGS_AND);

    for (;;)
      {
	  FAST GeAlarm *aptr = new GeAlarm(server,sys);

	  Alarm = aptr;

	  for (;;)
	    {
		FAST unsigned int flag = 
		  self.WaitOnFlags(ATASK_RESET | ATASK_SIGNAL,FLAGS_OR);

		if (flag == ATASK_RESET)
		  {
		      delete(aptr);
		      break;
		  }
		else
		  aptr->Post();
	    }
      }
}
