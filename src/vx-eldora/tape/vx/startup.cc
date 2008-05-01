/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
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

#define GBL_SCOPE
#include "global.hh"		// Where we define globals for RPC use only!

#include "Task.hh"

extern "C" {
#include "tapeGlobals.h"
#include "stdioLib.h"
#include "rpcLib.h"
void startRpc(void);		// So C code can call us.
};

static TapeStatus stat;		// Assured it's available.
static Task *RpcTask;
static void RpcLoop(Task &);

void startRpc(void)
{
    tapeStatus = &stat;

    Hdr = NULL;

    RpcTask = new Task((FUNCPTR)RpcLoop,NULL,0,2,6000);
}

void RpcLoop(Task &self)
{
    if (rpcTaskInit() == ERROR)
      {
	  fprintf(stderr,"Failed to initialize RpcLoop.\n");
	  return;
      }

    startControl();
    startHeader();

    svc_run();
}
