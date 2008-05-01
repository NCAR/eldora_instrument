/*
 *	$Id$
 *
 *	Module:		 tapeCtrlHandler.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1996/06/24  22:59:49  craig
// Initial revision
//
 *
 * description: This module handles both downloaded commands and requests
 *              for status. 
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

extern "C" {
#define TAPE_CTRL_SCOPE extern
#include "Header.h"
#include "Volume.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "CellSpacing.h"
#include "Parameter.h"
#include "NavDesc.h"
#include "InSitu.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "NavInfo.h"
#include "Ins.h"
#include "MiniRIMS.h"
#include "Gps.h"
#include "InSituData.h"
#include "tapeCommands.h"
#include "recordDef.h"
#include "recordGbl.h"
#include "tapeControl.h"
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
	  else if (cmd & INIT_UNIT)
	    {
		cmd &= ~INIT_UNIT;
		INIT_FLAG=1;
#ifdef PRNT
		cout << "INIT UNIT COMMAND RECEIVED" << endl;
#endif
	    }
	  else if (cmd & USE_OTHER_UNIT)
	    {
		cmd &= ~USE_OTHER_UNIT;
		OTHER_UNIT_FLAG=1;
#ifdef PRNT
		cout << "USE OTHER UNIT COMMAND RECEIVED" << endl;
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
