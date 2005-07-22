/*
 *	$Id$
 *
 *	Module:	mcpl_reset	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/05  16:26:17  thor
 * Initial revision
 *
 *
 * description: This module resets the Multi-Crate Pipeline (MCPL)
 *              node over the VME bus.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "mcplDef.h"
#include "mcplGbl.h"

mcpl_reset()
{

/* Define some general purpose variables */
    short timeout, status;

    printf("Am going to send a reset command to the mcpl node\n");

/* Check status register and print its contents */

    status = *status_reg;
    printf("Initial card status is: %x\n",status);

/* Send a reset command to the mcpl */

    *control_reg = RESET;
    *start_reg = 0x0001;

    timeout = 0;
    do
      {
          status = *status_reg;
          taskDelay(1);
      } while (((status & 0x0001) != 0) && (++timeout < 100));

    if(timeout >= 100)
      printf("MCPL card still busy after issueing reset command\n");
    else
      printf("MCPL card reset successful\n");

    status = *status_reg;
    printf("Final card status is: %x\n",status);
}



































