/*
 *	$Id$
 *
 *	Module:	sync.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1994/05/20  20:36:45  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:42:52  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to sync the time-of-day (tod)
 *              board's clock chip to the incomming IRIG-B signal coming
 *              from ADS (or any other source).
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

char sync_irig() 
{

unsigned char hseconds; 
short timeout;
short i;

/********** ENABLE LOADING OF IRIG REGISTER **************/

*irigb=0x00; /* Enable IRIGB data into registers */

/* Wait for last IRIG-B register to be loaded */

timeout = 0;
do{
    timeout++;
    taskDelay(1);
    }while(((*strg & LDDAY2) != LDDAY2) && (timeout < 90));
if(timeout >= 90) return((char)0);


/************** LOAD CLOCK REGISTERS WITH IRIG DATA ****************/

*tod_cmnd=0x04; /* Stop clock for loading,leave in 24 hr mode */
*hsec_reg=0x00;     /* Init hundreths to zero */
*sec_reg=1+((unsigned char)0x0F&*isec)+((*isec>>4)*10); /* ADD 1 second */

*min_reg=((unsigned char)0x0F&*imin)+((*imin>>4)*10); /* Load minute register
						     with IRIG minutes */

*hr_reg=((unsigned char)0x0F&*ihr)+((*ihr>>4)*10); /* Load hours register with
						  IRIG hours */

/*********** WAIT FOR TIME MARK BEFORE STARTING CLOCK ********/

*irigb=0x00;
timeout = 0;
do{
      timeout++;
      taskDelay(1);
  }while(((*strg & TMRKFND) != TMRKFND) && (timeout < 90));


/********* START CLOCK (Whether we were successful or not *******************/

*tod_cmnd=0x1c; /* Normal Mode, Ints enabled, 12 hr mode, 32.768kHz crystal */
if(timeout >= 90) return((char)0);

return((char)1);
}


