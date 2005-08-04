/*
 *	$Id$
 *
 *	Module:	pwr.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  17:36:42  reif
 * *** empty log message ***
 *
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
#include <pwrDef.h>
#include <pwrFunc.h>

void pwr()
{
    float xmit_csr_delay,testp_csr_delay;
    
    printf("INPUT PWR AND TP CURSOR DELAYS SEPARATED BY A COMMA\n");
    scanf(" %g,%g",&xmit_csr_delay,&testp_csr_delay);
    start_ieee(xmit_csr_delay,testp_csr_delay);
    while(1)
      {

	  if(xmit_isr_done==1)
	    {
		flt_pt();
		printf("FORE_XMIT_PWR=%-10.3E watts  AFT_XMIT_PWR=%-10.3E watts\r",fore_xmit_pwr,aft_xmit_pwr);
		xmit_isr_done=0;
	    }
	  else if(xmit_isr_done==-1)
	    {
		printf("NO DAMN GOOD\n");
		xmit_isr_done=0;
	    }
	  if(testp_isr_done==1)
	    {
		flt_pt(); 
		printf("FORE_TESTP_PWR = %-7.2f dBm  AFT_TESTP_PWR = %-7.2f dBm\r",fore_testp_pwr,aft_testp_pwr);
		testp_isr_done=0;
	    }
	  else if(testp_isr_done==-1)
	    {
		printf("NO DAMN GOOD\n");
		testp_isr_done=0;
	    }
      }
}
