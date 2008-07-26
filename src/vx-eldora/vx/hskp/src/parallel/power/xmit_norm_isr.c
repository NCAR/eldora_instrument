/*
 *	$Id$
 *
 *	Module:	xmit_norm_isr.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1997/11/12  19:49:44  eric
 * added semaphore passing.
 *
 * Revision 1.1  1993/09/20  17:35:42  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to show that data from the xmit
 *              power meter has been successfully transfered to system memory
 *              and is ready for conversion and retrieval.  
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "pwrDef.h"
#include "pwrFunc.h"
#include "semLib.h"

void xmit_norm_isr()
{
  semGive(xmit_data_sem);
  xmit_isr_done=1; /* Set global done flag to show data ready for retrieval */
  ieee_xmit_cnt++; /* Shows that the interrupts are still working */
  *d0csr=0xff; /* Clear channel status register */
  /*  *d0ccr=0x48; *//* Set continuous mode and interrupts keep on going */
} 















