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
 * Revision 1.6  1992/01/08  16:19:29  thor
 * Added code for timeout change.
 *
 * Revision 1.5  1991/10/17  17:15:45  thor
 * Removed actual reboot from this code. Cleaned up cases.
 *
 * Revision 1.4  1991/10/17  16:46:28  thor
 * Fixed cases for RESTART & RELOAD. Corrected to only copy DispCommand if
 * a new comes in.
 *
 * Revision 1.3  1991/10/15  15:38:00  thor
 * Fixed reversed order of bcopy.
 *
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
 * action. Getstatus_1 simply returns the latest status.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

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
      {
	  cmd->cmd -= LOAD_ONLY;
	  memcpy((char *)GeCommand,(char *)cmd,sizeof(DispCommand));
	  DrawingTask->SetFlags(LOAD_ONLY);
	  status->status = IDLE;
      }
    else
      {
	  switch(command)
	    {
	      case REBOOT:
	      case STOP:
		DrawingTask->SetFlags(command);
		status->status = IDLE;
		break;
		
	      case RELOAD:
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case START:
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case RESTART:
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case FORWARD_RADIAL:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case FORWARD_HORIZ:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case FORWARD_VERT:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case AFT_RADIAL:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case AFT_HORIZ:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	      case AFT_VERT:
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
