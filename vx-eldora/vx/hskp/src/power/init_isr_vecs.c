/*
 *	$Id$
 *
 *	Module:init_isr_vecs.c	 
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
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void init_isr_vecs()
{
/**************** INITIALIZE ISR VECTORS *****************/

  if(intConnect((VOIDFUNCPTR *)(XMIT_NORM_INT*4),(VOIDFUNCPTR)xmit_norm_isr,0)==ERROR)
    {
	printf("FAILED TO CONNECT. EXITING\n");
	exit(1);
    }
  if(intConnect((VOIDFUNCPTR *)(XMIT_ERR_INT*4),(VOIDFUNCPTR)xmit_err_isr,0)==ERROR)
    {
	printf("FAILED TO CONNECT. EXITING\n");
	exit(1);
    }
  if(intConnect((VOIDFUNCPTR *)(TESTP_NORM_INT*4),(VOIDFUNCPTR)testp_norm_isr,0)==ERROR)
    {
	printf("FAILED TO CONNECT. EXITING\n");
	exit(1);
    }
  if(intConnect((VOIDFUNCPTR *)(TESTP_ERR_INT*4),(VOIDFUNCPTR)testp_err_isr,0)==ERROR)
    {
	printf("FAILED TO CONNECT. EXITING\n");
	exit(1);
    }
}
