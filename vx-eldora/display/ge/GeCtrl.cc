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
 * Revision 2.3  1994/09/07  17:10:48  thor
 * Moved to new version of RPC.
 *
 * Revision 2.2  1993/09/28  13:04:33  thor
 * Added dual display support.
 *
 * Revision 2.1  1993/08/20  17:17:03  thor
 * Commented out alarm code & renamed RESTART RESTART_DISP.
 *
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.8  1992/10/21  14:33:06  thor
 * Fixed LOAD_ONLY cose to propigate command.
 *
 * Revision 1.7  1992/10/02  20:42:24  thor
 * Added LOAD_ONLY support.
 *
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
extern "C" {
#include "string.h"
};

static u_long cmdCount = 0;

struct DispStatus *senddispcommand_1_svc(FAST DispCommand *cmd,
                                         struct svc_req *req)
{
    FAST u_long command = cmd->cmd;
    FAST DispStatus *status = GeStatus;

    if (command & LOAD_ONLY)
      {
	  command &= ~LOAD_ONLY;
	  memcpy((char *)GeCommand,(char *)cmd,sizeof(DispCommand));
	  GeCommand->cmd = command; // It seems we cannot alter cmd.
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
		
	      case START:
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;
		
	      case FORWARD_RADIAL:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;
		
	      case FORWARD_HORIZ:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;
		
	      case FORWARD_VERT:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;
		
	      case AFT_RADIAL:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;
		
	      case AFT_HORIZ:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;
		
	      case AFT_VERT:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;

	      case FORWARD_DUAL:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;

	      case AFT_DUAL:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  status->status = DRAWING;
                  break;

	      case TMO_CHANGE:
                  memcpy(GeCommand,cmd,sizeof(DispCommand));
                  DrawingTask->SetFlags(command);
                  break;
	    }
      }
    return(status);
}

DispStatus *getstatus_1_svc(void *nought, struct svc_req *req)
{
    return(GeStatus);
}
