/*
 *	$Id$
 *
 *	Module:	test_pwr.c	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Initial revision
 *
 *
 * description: The purpose of this module is to test the GPIB interface
 *              to the power meters independently of the rest of the
 *              Housekeeper code. Start and stop the system once then
 *              run test_pwr. MyD can be varied to test performance.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
int MyD;
#include "hskpAll.h"
#include "semLib.h"

void test_pwr()
{
  MyD = 1000;
  start_ieee();
  while(MyD != 0)
    {
      taskDelay(MyD);
      New_tp = 1;           /* handshake with Test Pulse Power Meter */
      semGive(xmit_pwr_sem); /* handshake with Transmit Power Meter */
    }
  stop_ieee();
} 















