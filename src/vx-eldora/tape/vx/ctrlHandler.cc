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

#define TAPE_CTRL_SCOPE
extern "C" {
#include "tapeGlobals.h"
#include "rebootLib.h"
#include "sysLib.h"
#include "stdioLib.h"
};

TapeStatus *sendcommand_1(FAST TapeCommand *command, struct svc_req *req)
{
    FAST u_long cmd = command->cmd; /* The command. */
    FAST TapeStatus *ptr = tapeStatus; /* Our global status. */
    FAST u_long *stat = (u_long *)&(ptr->status);

    static u_long count = 0;	/* The running count of commnads. */

    if (cmd == INIT)		/* Indicates that other side has restarted. */
      {
	  count = 0;		/* Back to beginning! */
	  PACKSTATUS(IDLE,stat,0);
	  PACKSTATUS(IDLE,stat,1);
	  ptr->count = 1;
	  return(ptr);
      }

    if (count >= command->count) /* Stale command. */
      return(ptr);

    count = command->count;

    while (cmd)
      {
	  if (cmd & REBOOT)
	    reboot(BOOT_NORMAL); /* Wham! */
	  else if (cmd & STOP)
	  {
	      printf("Stopping\n");
	      cmd &= ~STOP;
	      PACKSTATUS(IDLE,stat,command->unit);
	      ptr->count++;
	  }
	else if (cmd & START)
	  {
	      printf("Starting\n");
	      cmd &= ~START;
	      PACKSTATUS(RUNNING,stat,command->unit);
	      ptr->count++;
	  }
	else if (cmd & REC_ON)
	  {
	      printf("Recording on\n");
	      cmd &= ~REC_ON;
	      PACKSTATUS(RECORDING,stat,command->unit);
	      RecordingFlag = 1;
	      ptr->count++;
	  }
	else if (cmd & REC_OFF)
	  {
	      printf("End recording\n");
	      cmd &= ~REC_OFF;
	      PACKSTATUS(RUNNING,stat,command->unit);
	      RecordingFlag = 0;
	      ptr->count++;
	  }
	else if (cmd & REWIND)
	  {
	      printf("Rewinding\n");
	      cmd &= ~REWIND;
	      PACKSTATUS(REWINDING,stat,command->unit);
	      ptr->count++;
	  }
	else if (cmd & EJECT)
	  {
	      printf("Ejecting from unit %d\n",command->unit);
	      cmd &= ~EJECT;
	      PACKSTATUS(EJECTING,stat,command->unit);
	      ptr->count++;
	  }
	else if (cmd & SET_UNIT)
	  {
	      printf("Set unit\n");
	      cmd &= ~SET_UNIT;
	      ptr->unit = command->unit;
	  }
	else if (cmd & PP_ON)
	  {
	      printf("Ping pong on\n");
	      cmd &= ~PP_ON;
	      PingPongFlag = 1;
	  }
	else if (cmd & PP_OFF)
	  {
	      printf("Ping pong off\n");
	      cmd &= ~PP_OFF;
	      PingPongFlag = 0;
	  }
    }
    return(ptr);
}

TapeStatus *gettapestatus_1(void *x, struct svc_req *req)
{
    return(tapeStatus);
}
