/*
 * $Id$
 *
 * Module: vmevme_isr  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log: vmevme_isr.c,v $
 * Revision 1.12  2005/08/04 22:27:36  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.11  1997/08/27 16:50:02  craig
 * *** empty log message ***
 *
 * Revision 1.10  1996/02/09  18:29:26  craig
 * *** empty log message ***
 *
 * Revision 1.9  1994/05/20  20:37:07  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  20:44:35  craig
 * Initial revision
 *
 *
 * description: This module answers the interrupt caused by the fore
 *              radar vme to vme interface.  This is the middle of beam
 *              interrupt. This routine handles both the fore and aft
 *              radar processors.
 *
 */

static char rcsid[] = "$Date$ $RCSfile: vmevme_isr.c,v $ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void vmevme_isr()
{

/* This services the the middle of beam interrupt caused by the fore radar
   processor but handles both processors, first clear the interrupts. */

//*fore_remote_command = 0x40; 
//*aft_remote_command = 0x40;
// Tom 3/24/08
*clk_intstat = 0x01;

/* Check to see if it is OK to let the other radar processor use the
   mcpl bus */
/*
if(fore_vmehndshk->mcpl_hndshk == 0 && mcpl_xfer_processor == 0)
  {
    aft_vmehndshk -> mcpl_hndshk = 1;
    mcpl_xfer_processor = 1;
  }
if(aft_vmehndshk->mcpl_hndshk == 0 && mcpl_xfer_processor == 1)
  {
    fore_vmehndshk -> mcpl_hndshk = 1;
    mcpl_xfer_processor = 0;
  }
*/
/* Now, clear the interrupts one more time to make sure. */

//*fore_remote_command = 0x40; 
//*aft_remote_command = 0x40;
 

/* Now send a semifore to start the midbeam task */

semGive(vmeSem);

}
