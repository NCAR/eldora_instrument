/*
 * $Id$
 *
 * Module: init_clock.c  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log: init_clock.c,v $
 * Revision 1.3  2005/08/04 22:27:38  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.2  2003/09/25  17:25:13  kapoor
 * change of time global variable names
 *
 * Revision 1.1  2003/09/24  20:52:51  kapoor
 * Initial revision
 *
 * Revision 1.2  2003/09/23  22:58:15  kapoor
 * changed include to "hskpAll.h"
 *
 * Revision 1.1  2003/09/23  16:42:57  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:32:42  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:36  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.7  1998/09/08  17:34:25  thor
 * *** empty log message ***
 *
 * Revision 1.6  1996/02/15  20:41:00  craig
 * Added calibrations to the analog to digital and digital to analog stuff
 *
 * Revision 1.5  1995/10/27  20:48:16  martin
 * changed HB to HBM
 *
 * Revision 1.4  1995/09/25  17:44:23  martin
 * updated from Craig's current code
 *
 * Revision 1.3  1995/09/25  15:35:19  craig
 * *** empty log message ***
 *
 * Revision 1.2  1995/08/14  14:43:53  craig
 * *** empty log message ***
 *
 * Revision 1.1  1995/07/11  21:04:54  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to initialize all of the
 *              global variables used with the Bancomm clock
 *              board.  It also corrects the day/month calculation
 *              array for leap yr
 */

static char rcsid[] = "$Date$ $RCSfile: init_clock.c,v $ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void init_clock(short div_525) 
{

timereq = (short *)(SHORT_BASE + BASE_CLOCK + TIMEREQ);
clk_ack = (short *)(SHORT_BASE + BASE_CLOCK + CLK_ACK);
clk_cmd = (short *)(SHORT_BASE + BASE_CLOCK + CLK_CMD);
clk_intstat = (short *)(SHORT_BASE + BASE_CLOCK + CLK_INTSTAT);
clk_fifo = (short *)(SHORT_BASE + BASE_CLOCK + CLK_FIFO);
//Tom 3/24/08
clk_vector = (short *)(SHORT_BASE + BASE_CLOCK + CLK_VECTOR);
clk_level = (short *)(SHORT_BASE + BASE_CLOCK + CLK_LEVEL);
clk_mask = (short *)(SHORT_BASE + BASE_CLOCK + CLK_MASK);


// Initialize the clock card to receive external interrupt
// code from 5-2 of Symmetricom Bc635VME manual
*clk_cmd = 0x8;                         // Enable external event 
*clk_vector = VME_VME_VEC;              // Map the VME interrupt vector
*clk_level = VME_VME_IRQ;               // Set the VME IRQ level
*clk_intstat = 0x01;                     // Clear the INSTAT bit
*clk_mask = 0x1;                        // Enable the external interrupt



/* Set to sync to IRIGB or the real time clock (RTC) */

if(use_IRIGB)
  {
      /* Use IRIG-B as the time source */
      send_packet("A0");
      if(modulate_IRIGB)
 send_packet("HBM");
      else
 send_packet("HBD");
  }
else
  {
      /* Use the real time clock as the time source */
      send_packet("A3");
  }

day_mon_calc[0] = 31;    /* jan */
day_mon_calc[1] = 28;    /* feb */
day_mon_calc[2] = 31;    /* mar */
day_mon_calc[3] = 30;    /* apr */
day_mon_calc[4] = 31;    /* may */
day_mon_calc[5] = 30;    /* june */
day_mon_calc[6] = 31;    /* july */
day_mon_calc[7] = 31;    /* aug */
day_mon_calc[8] = 30;    /* sept */
day_mon_calc[9] = 31;    /* oct */
day_mon_calc[10] = 30;   /* nov */
day_mon_calc[11] = 31;   /* dec */
day_mon_calc[12] = 0;   /* place holder */

if(((int)(yr/4))*4 == yr)
  day_mon_calc[1] = 29;

return;
}






