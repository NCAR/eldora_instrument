/*
 *	$Id$
 *
 *	Module:	vmevme_isr	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
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

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void vmevme_isr()
{

/* This services the the middle of beam interrupt caused by the fore radar
   processor but handles both processors, first clear the interrupts. */

*fore_remote_command = 0x40; 
*aft_remote_command = 0x40; 

/* Now call the subroutine that does everything else, need to use
   a subroutine since it does a lot of floating point */

isr_func();


}
