
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
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define VXWORKS

#include "Task.hh"
#include "stdioLib.h"
#include "rpcLib.h"
#include "HeaderRpc.h"

extern "C" {
#include "tapeStatus.h"
#include "tapeControl.h"
#include "tapeCtrlGbl.h"
void startRpc(void);		// So C code can call us.
};

static TapeStatus stat;
static Task *RpcTask;
static void RpcLoop(Task &);

void startRpc(void)
{
    tapeStatus = &stat;

    Hdr = NULL;

    RpcTask = new Task((FUNCPTR)RpcLoop,NULL,0,2,18000);
}

void RpcLoop(Task &self)
{
    if (rpcTaskInit() == ERROR)
      {
	  fprintf(stderr,"Failed to initialize RpcLoop.\n");
	  return;
      }

    startControl();
    HeaderRpcInit();

    svc_run();
}

