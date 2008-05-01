/*
 *	$Id$
 *
 *	Module:	xmit_err_isr.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1997/11/12  19:49:28  eric
 * Added semaphore passing and error status.
 *
 * Revision 1.1  1993/09/20  17:35:46  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to show that an error has 
 *              occured in retrieving data from the xmit power meter.  It also
 *              tries to recover by clearing the channel status register
 *              and trying to continue with the next retrieval.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "pwrDef.h"
#include "pwrFunc.h"
#include "semLib.h"

void xmit_err_isr()
{
  semGive(xmit_err_sem);
  xmit_isr_done=-1; /* Shows that there is an error */
  dma_err1 = *d0cer; /* Get error status */
  *d0csr=0xff; /* Clear channel status register */
  /*  *d0ccr=0x48; *//* Try to keep on going if possible */
}



