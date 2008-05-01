/*
 *	$Id$
 *
 *	Module: SAM4		 
 *	Original Author: Grant Gray 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/08/24  16:31:39  eric
 * added "silent" mode to supress diagnostic
 * printf's.
 *
 * Revision 1.1  1992/11/09  22:58:28  eric
 * Initial revision
 * 
 *
 *
 * description:
 *
 *  Changes sample size stored in DSP location 0x52.
 *  arg1 = number of samples;
 *  arg2 = number of PPP's and DP's to program
 *  arg3 = frequency; 1,2, or 3
 *  arg4 = board
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "stdarg.h"
#include "ctype.h"
#ifndef NU_ARCHIT
#include "ELDRP7.h"
#endif
#ifdef NU_ARCHIT
#include "NU_ELDRP7.h"
#endif
 
unsigned long getaddr();
unsigned int  load4();
unsigned int num_samples; 
extern int Silent;
int sam4(int sampl)


{
    va_list ap;
    unsigned long  bd, frq, dspc;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status;
    int n, na;
    
    va_start(ap,sampl);
    num_samples = sampl;   /* Read in no. samples */
    val = num_samples;
    n = va_arg(ap, int);
    for(na=0; na < n; na++)
      {
	  frq = va_arg(ap, int);            /* Read in Frequency */
	  frq = (frq << 24) + EXTD_BASE;     /* freq # * 10**6 + EXTD_BASE */
	  bd = va_arg(ap, int);             /* Read in Board */
	  bd = bd << 20;                    /* Board # * 10**5 = base addr */
	  bd += frq;                        /* Full base addr of board */
 
	  for(i=0;i<4;i++)
	    {
		pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x52;
		*(pio + PARH) = 0;
		*(pio + PDRL) = val & 0xFF;
		*(pio + PDRH) = (val >> 8) & 0xFF;

		/* check that correct number of samples were written */
		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x52;
		*(pio + PARH) = 0;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		if(status != val)
		  if(!Silent)
		    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	    }
 
      }
    va_end(ap);

/*   Now for the Collator */

     bd = COL0BASE;
     pio = (unsigned char *)(bd +  DSPSEL);
     if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = 0x0;
     *(pio + PARL) = 0x52;
     *(pio + PARH) = 0x0;
     *(pio + PDRL) = val & 0xFF;
     *(pio + PDRH) = (val >> 8) & 0xFF;
    /* check that correct number of samples were written */
    *(pio + PARE) = 0x0;
    *(pio + PARL) = 0x52;
    *(pio + PARH) = 0;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      if(!Silent)
	printf("PIO R/W ERROR in Collator, Processor 0 \n");
}



