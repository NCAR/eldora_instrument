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
};

static void RpcLoop(void);


static DispStatus ge_status;

void GeStart(char *server, int sys)
{
    // Put hardware initialization, etc. here.

    system = sys;

    Alarm = NULL;

    GeStatus = &ge_status;

    ge_status.status = LOADED;
    ge_status.count = 0;

    // Now we start Rpc services.

    if (taskSpawn("RpcLoop",2,0,4000,(FUNCPTR)RpcLoop) == ERROR)
      fprintf(stderr,"Failed to start RpcLoop!!!\n");

      if (taskSpawn("Alarm",2,0,4000,(FUNCPTR)AlarmLoop,(int)server,sys)
	  == ERROR)
	fprintf(stderr,"Failed to start Alarm loop!!!\n");
}

static void RpcLoop(void)
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

void AlarmLoop(char *server, int sys)
{
    GeAlarm alarm(server,sys);

    Alarm = &alarm;
    
    for (;;)
      alarm.Wait();
}
