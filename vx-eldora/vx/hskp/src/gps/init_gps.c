/*
 *	$Id$
 *
 *	Module: gpsInit.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  17:23:56  craig
 * Initial revision
 *
 *
 * description:  Initializes the gps handling pointers etc.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void init_gps(short mbox)
{

/* Set the pointers to the gps data areas */

gps_status = (char *)(GPS_STATUS + STANDARD_BASE + GPS_BASE);
gps_command = (short *)(GPS_COMMAND + STANDARD_BASE + GPS_BASE);
gps_mbox = (short *)(GPS_MBOX + STANDARD_BASE + GPS_BASE);
gps_hndshk = (short *)(GPS_HNDSHK + STANDARD_BASE + GPS_BASE);
gps_d_loc_h = (short *)(GPS_D_LOC_H + STANDARD_BASE + GPS_BASE);
gps_d_loc_l = (short *)(GPS_D_LOC_L + STANDARD_BASE + GPS_BASE);

/* Command interface to use the mailbox interrupt (mbox!=0) or not (mbox==0) */

*gps_mbox = mbox;

/* Set the gps status byte to indicate no errors */

currStatus->gps = 0;

/* Clear the global GPS interrupt detected flag */
in_gps_isr = 0;

/* Setup the TP41V to handle mailbox interrupts */

wcio(0,"a",0xc6);  /* Writes correct register on the TP41V to allow
                      dual porting the memory */

/* Dual port the top 2 MBytes of DRAM to VME addresses 0x200000 thru
   0x3FFFFF, also creates mailbox interrupts in top 16 KBytes */

createStdMailbox((long)0x40200000,(long)0x200000);

/* Connect the interrupt handler to the first mailbox interrupt */

connectMailbox((VOIDFUNCPTR)gps_isr,0,0);

/* Create pointer to the mailbox address the gps card will use and clear
   that location */

tp41_mbox_0 = (char *)(DRAM_BASE + TP41_MBOX_0);
*tp41_mbox_0 = 0;

return;
}
