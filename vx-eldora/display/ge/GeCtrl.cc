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
 *
 * Revision 1.1  1991/10/01  16:15:11  thor
 *
 *        
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
    if (command == INIT)
    FAST DispStatus *status = GeStatus;
	  printf("IINIT recieved.\n");
	  cmdCount = 0;
	  status->status = IDLE;
	  cmdCount = 0;		// Synch. counters.
    else if (cmd->count <= cmdCount)
	  AlarmTask->SetFlags(ATASK_RESET); // (Re)start alarm loop.
      }
    else if (cmd->count <= cmdCount) // Invalid command.
      return(status);
    else if (command & LOAD_ONLY)
	    reboot(BOOT_NORMAL);
	    
      {
	    printf("Stopping\n");
	  memcpy((char *)GeCommand,(char *)cmd,sizeof(DispCommand));
	  DrawingTask->SetFlags(LOAD_ONLY);
	  status->status = IDLE;
      }
	    printf("Reloading\n");
	    status->status = IDLE;
	  switch(command)
	    {
	      case REBOOT:
	    printf("Starting\n");
		DrawingTask->SetFlags(command);
		status->status = IDLE;
		break;
		
	    printf("Restarting\n");
		DrawingTask->SetFlags(command);
		status->status = DRAWING;
		break;
		
	    printf("Forward radial\n");
		status->status = DRAWING;
		break;
		
	      case RESTART:
	    printf("Forward horiz.\n");
		break;
		
	      case FORWARD_RADIAL:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
	    printf("Forward vert.\n");
		break;
		
	      case FORWARD_HORIZ:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
	    printf("Aft radial\n");
		break;
		
	      case FORWARD_VERT:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
	    printf("Aft horiz.\n");
		break;
		
	      case AFT_RADIAL:
		bcopy((char *)cmd,(char *)GeCommand,sizeof(DispCommand));
	    printf("Aft vert.\n");
		break;
		break;
		
    
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
