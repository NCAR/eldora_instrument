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
 * Revision 2.5  1994/11/01  17:58:09  thor
 * Switched to C++ I/O, added name for RPC task.
 *
 * Revision 2.4  1994/09/12  18:44:18  thor
 * Removed alarm stuff.
 *
 * Revision 2.3  1994/04/08  20:55:28  thor
 * Added iostream support, increased stack sizes.
 *
 * Revision 2.2  1993/08/20  17:17:03  thor
 * Renamed system to dispsystem. Commented out alarm code.
 *
 * Revision 2.1  1993/07/01  17:21:58  thor
 * Fixed incorrect cast.
 *
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
#define WINDOW_GBLS_SCOPE
#include "GeGlobal.hh"
#include "HeaderRpc.h"

#include <iostream.h>
#include <rpcLib.h>
#include "tp41Lib.h"
#include <sysLib.h>

extern "C" { void __do_global_ctors(void); };

static DispStatus ge_status;
static DispCommand ge_command;
static Beam_Time lasttime;

void GeStart()
{
  // Put hardware initialization, etc. here.

  sysIntEnable(1);		// Enable interrupts.
  sysIntEnable(2);
  sysIntEnable(3);

  dport((char *)0x40200000,(void *)0x10000000,4); // Set up dual
  // ported memory.

  wcio(0,"a",0xc8);		// Set VME page & attach VME ext space.

  GeStatus = &ge_status;
  GeCommand = &ge_command;
  LastTime = &lasttime;

  ge_status.status = LOADED;

  // Initialize all globally declared objects.
  __do_global_ctors();

  Hdr = NULL;

  // Start graphics task.
  DrawingTask = new Task((FUNCPTR)DrawingLoop,NULL,0,DRAWING_PRI,60000);

  // Now we start control loop.
  CtrlTask = new Task((FUNCPTR)RpcLoop,NULL,0,CTRL_PRI,60000,0,1,"RpcLoop");
}

void RpcLoop(Task &self)
{ 
  if (rpcTaskInit() == ERROR)
    {
      cout << "Failed to initialize Rpc." << endl;
      return;
    }

  DispRpcInit();

  HeaderRpcInit();

  svc_run();
}
