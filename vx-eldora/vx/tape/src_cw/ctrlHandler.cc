/*
 *	$Id$
 *
 *	Module:		 tapeCtrlHand.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1992/10/25  17:08:21  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:34:11  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:47  reif
 * *** empty log message ***
 *
 * Revision 1.1  1991/09/11  19:35:49  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This module handles both downloaded commands and requests
 * for status. 
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
extern "C"{
#include <cipincl.h>
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
	      cmd &= ~STOP;
	      PACKSTATUS(IDLE,stat,command->unit);
	      RUN_FLAG=0;
	      printf("SYSTEM STOPPED\n");
	      ptr->count++;
	  }
	else if (cmd & START)
	  {
	      printf("SYSTEM STARTED\n");
	      cmd &= ~START;
	      PACKSTATUS(RUNNING,stat,command->unit);
	      RUN_FLAG=1;
	      ptr->count++;
	  }
	else if (cmd & REC_ON)
	  {
	      cmd &= ~REC_ON;
	      PACKSTATUS(RECORDING,stat,command->unit);
	      REC_FLAG = 1;
	      printf("RECORD ON\n");
	      ptr->count++;
	  }
	else if (cmd & REC_OFF)
	  {
	      cmd &= ~REC_OFF;
	      PACKSTATUS(RUNNING,stat,command->unit);
	      REC_FLAG = 0;
	      printf("RECORD OFF\n");
	      ptr->count++;
	  }
	else if (cmd & REWIND)
	  {
	      cmd &= ~REWIND;
	      PACKSTATUS(REWINDING,stat,command->unit);
	      REWIND_FLAG=1;
	      printf("REWINDING\n");
	      ptr->count++;
	  }
	else if (cmd & EJECT)
	  {
	      cmd &= ~EJECT;
	      PACKSTATUS(EJECTING,stat,command->unit);
	      UNLOAD_FLAG=1;
	      printf("EJECTING\n");
	      ptr->count++;
	  }
	else if (cmd & SET_UNIT)
	  {
	      printf("CHANGING UNIT\n");
	      cmd &= ~SET_UNIT;
	      ptr->unit = command->unit;
	      UNIT_NUM=command->unit;
	  }
	else if (cmd & PP_ON)
	  {
	      printf("PING PONG ON\n");
	      cmd &= ~PP_ON;
	      PP_FLAG = 1;
	  }
	else if (cmd & PP_OFF)
	  {
	      printf("PING PONG OFF\n");
	      cmd &= ~PP_OFF;
	      PP_FLAG = 0;
	  }
	else if (cmd & PAR_ON)
	  {
	      printf("PARALLEL ON\n");
	      cmd &= ~PAR_ON;
	      PARALLEL_REC = 1;
	  }
	else if (cmd & PAR_OFF)
	  {
	      printf("PARALLEL OFF\n");
	      cmd &= ~PAR_OFF;
	      PARALLEL_REC = 0;
	  }
	else if (cmd & CLIP)
	  {
	      printf("CLIPPING\n");
	      cmd &= ~CLIP;
	      CLIPPING = 0;
	  }
	else if (cmd & PIE_SLICE)
	  {
	      printf("SLICING OFF\n");
	      cmd &= ~PIE_SLICE;
	      SLICE = 0;
	  }
	else if (cmd & THRESHOLD)
	  {
	      printf("THRESHOLD OFF\n");
	      cmd &= ~THRESHOLD;
	      THRSHLD = 0;
	  }
    }
    return(ptr);
}

TapeStatus *gettapestatus_1(void *x, struct svc_req *req)
{
    return(tapeStatus);
}

