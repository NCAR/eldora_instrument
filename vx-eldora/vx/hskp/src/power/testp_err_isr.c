/*
 *	$Id$
 *
 *	Module:	testp_err_isr.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1997/11/12  19:48:37  eric
 * Added semaphore passing and error status.
 *
 * Revision 1.1  1993/09/20  17:35:50  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "semLib.h"
#include "pwrDef.h"
#include "pwrFunc.h"

void testp_err_isr()
{
  semGive(testp_err_sem);
  testp_isr_done=-1;
  dma_err2 = *d1cer; /* Get dma error status */
  *d1csr=0xff;      /* Clear dma status register */
  /*  *d1ccr=0x48; */
}

