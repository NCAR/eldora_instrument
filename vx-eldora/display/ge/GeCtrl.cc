/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeCtrl.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/10/14  19:20:00  thor
 * Added task control calls.
 *
 * Revision 1.1  1991/10/01  16:15:11  thor
 * Initial revision
 *
 *
 *
 * description:
 *        These functions are called by the underlying rpc code as
 * needed. Sendcommand_1 takes a command and invokes the appropriate
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
extern "C" {
#include "rebootLib.h"
#include "sysLib.h"
#include "stdioLib.h"
};


#include "GeGlobal.hh"

static u_long cmdCount = 0;

struct DispStatus *sendcommand_1(FAST DispCommand *cmd, struct svc_req *req)
{
    FAST u_long command = cmd->cmd;
    FAST DispStatus *status = GeStatus;

    if (command == INIT)	// UNIX side has (re)started.
      {
	  cmdCount = 0;		// Synch. counters.
	  status->count++;
	  AlarmTask->SetFlags(ATASK_RESET); // (Re)start alarm loop.
      }
    else if (cmd->count <= cmdCount) // Invalid command.
      return(status);
    else if (command & LOAD_ONLY)
	    reboot(BOOT_NORMAL);
	    
      {
	  cmd->cmd -= LOAD_ONLY;
	  memcpy((char *)GeCommand,(char *)cmd,sizeof(DispCommand));
	  DrawingTask->SetFlags(LOAD_ONLY);
	  status->status = IDLE;
      }
	    DrawingTask->SetFlags(STOP);
	    DrawingTask->SetFlags(START);
	    status->status = IDLE;
	  switch(command)
	    {
	      case REBOOT:
	    DrawingTask->SetFlags(START);
		DrawingTask->SetFlags(command);
		status->status = IDLE;
		break;
		
	    DrawingTask->SetFlags(STOP);
	    DrawingTask->SetFlags(START);
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case RESTART:
		status->status = DRAWING;
		break;
		
	      case FORWARD_RADIAL:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		status->status = DRAWING;
		break;
		
	      case FORWARD_HORIZ:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		status->status = DRAWING;
		break;
		
	      case FORWARD_VERT:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		status->status = DRAWING;
		break;
		
	      case AFT_RADIAL:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		status->status = DRAWING;
		break;
		break;
		
	      case AFT_VERT:
    bcopy((char *)GeCommand,(char *)cmd,sizeof(DispCommand));
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case TMO_CHANGE:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		break;
	    }
      }
    status->count++;
    
    return(status);
}

DispStatus *getstatus_1(void *nought, struct svc_req *req)
{
    GeStatus->count++;

    return(GeStatus);
}
