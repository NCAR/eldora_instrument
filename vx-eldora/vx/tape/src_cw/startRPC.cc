
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
// Revision 1.2  1994/11/14  17:50:08  craig
// Modified for new control processor software
//
// Revision 1.1  1994/01/06  21:31:45  craig
// Initial revision
//
 * Revision 1.3  1992/10/25  17:08:22  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:49  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Task.hh"

#include "stdioLib.h"
#include "rpcLib.h"
#include "HeaderRpc.h"

extern "C" {
#include "tapeGlobals.h"
#include "tapeControl.h"
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
