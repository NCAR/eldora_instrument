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
 * Revision 1.1  1994/01/06  21:31:44  craig
 * Initial revision
 *
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

extern "C" {
#define TAPE_CTRL_SCOPE extern
#include <tapeGlobals.h>
#include <tapeControl.h>
};

TapeStatus *sendcommand_1_svc(FAST TapeCommand *command, struct svc_req *req)
{
    FAST u_long cmd = command->cmd; /* The command. */
    FAST TapeStatus *ptr = tapeStatus; /* Our global status. */
    FAST u_long *stat = (u_long *)&(ptr->status);

    while (cmd)
      {
	  if (cmd & STOP)
	    {
		cmd &= ~STOP;
		RUN_FLAG=0;
#ifdef PRNT
		cout << "SYSTEM STOP COMMAND RECEIVED" << endl;
#endif
	    }
	  else if (cmd & START)
	    {
#ifdef PRNT
		cout << "SYSTEM START COMMAND RECEIVED" << endl;
#endif
		cmd &= ~START;
		RUN_FLAG=1;
	    }
	  else if (cmd & REC_ON)
	    {
		cmd &= ~REC_ON;
		REC_FLAG = 1;
#ifdef PRNT
		cout << "RECORD COMMAND RECEIVED" << endl;
#endif
	    }
	  else if (cmd & REC_OFF)
	    {
		cmd &= ~REC_OFF;
		REC_FLAG = 0;
#ifdef PRNT
		cout << "RECORD OFF COMMAND RECEIVED" << endl;
#endif
	    }
	  else if (cmd & REWIND)
	    {
		cmd &= ~REWIND;
		REWIND_FLAG=1;
#ifdef PRNT
		cout << "REWIND COMMAND RECEIVED" << endl;
#endif
	    }
	  else if (cmd & EJECT)
	    {
		cmd &= ~EJECT;
		UNLOAD_FLAG=1;
#ifdef PRNT
		cout << "EJECT COMMAND RECEIVED" << endl;
#endif
	    }
      }
    return(ptr);
}

TapeStatus *gettapestatus_1_svc(void *x, struct svc_req *req)
{
    return(tapeStatus);
}
