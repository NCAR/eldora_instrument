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

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void xmit_err_isr()
{
    xmit_isr_done=-1; /* Shows that there is an error */
    *d0csr=0xff; /* Clear channel status register */
    *d0ccr=0x48; /* Try to keep on going if possible */
}
