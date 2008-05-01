/*
 *	$Id$
 *
 *	Module: PROBE		 
 *	Original Author: Grant Gray 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  22:58:30  eric
 * Initial revision
 *
 *
 *
 * description:
 *
 *  Reads several key DSP locations.
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
#include "ELDRP7.h"
 
unsigned long getaddr();
unsigned int  load4();
unsigned int num_samples; 
int probe(rpt)
int rpt;


{
    va_list ap;
    unsigned long  bd, frq, dspc;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status;
    int n, na;
    
    va_start(ap,rpt);
    n = rpt;
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

		/* Dump key values to screen */

		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x50;
		*(pio + PARH) = 0;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*gates = %x \n",status);
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*samples = %x \n",status);

		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x12;
		*(pio + PARH) = 0x1;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*prt_flag = %x \n",status);
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*f1_flag = %x \n",status);
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*f2_flag = %x \n",status);
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*f3_flag = %x \n",status);
		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x98;
		*(pio + PARH) = 0x4;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		printf("*in_loop = %x \n",status);
	    }
 
      }
    va_end(ap);
}



