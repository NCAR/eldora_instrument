/*
 *	$Id$
 *
 *	Module:	PKGATE	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 *
 * description:
 *
 *  Loads the gates variable into four DSP's and the collator.
 *  arg1 = Pulse Pair Processor picked gate (for time series)
 *  arg2 = number of boards to program
 *  arg3 = frequency; 1,2, or 3
 *  arg4 = board      
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 
#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "varargs.h"
#include "ELDRP7.h"
unsigned int pick_gate;  

int pkgate(va_alist)

va_dcl

{
  va_list ap;
  unsigned long  bd, frq, dspc;
  unsigned char *pio, scr, *temp;
  unsigned int valc, vald, i, status;
  int n, na;

  va_start(ap);
  pick_gate = va_arg(ap,int);    /* Read in number of gates */
  vald = pick_gate;
  n = va_arg(ap, int);
  for(na = 0; na < n; na++)
    {
	frq = va_arg(ap, int);          /* freq 1,2, or 3 */
	frq = (frq << 24) + EXTD_BASE;   /* freq # * 10**6 + EXTD_BASE*/
	bd = va_arg(ap, int);           /* board number */
	bd = bd << 20;                  /* Board # * 10**5 = base addr */
	bd += frq;                      /* Full base addr. of board */
	for(i=0;i<4;i++)
	  {
	      pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	      if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	      *(pio + PARE) = 0x0;
	      *(pio + PARL) = 0x6C;
	      *(pio + PARH) = 0;
	      *(pio + PDRL) = vald & 0xFF;
	      *(pio + PDRH) = (vald >> 8) & 0xFF;
	      /* Test that number of gates written is correct */
	      *(pio + PARE) = 0x0;
	      *(pio + PARL) = 0x6C;
	      *(pio + PARH) = 0;
	      temp = (unsigned char *)(pio + PDRL);
	      status = *temp & 0xff;
	      temp = (unsigned char *)(pio + PDRH);
	      status += (*temp & 0xff)<< 8;
	      if(status != vald)
		printf("PIO R/W Error in Board %x, Processor %d \n", bd,i);
          }
    }
  va_end(ap);
}



























