/*
 *	$Id$
 *
 *	Module:	DSPSTOP	 
 *	Original Author: Grant Gray 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *
 *  Stops  DSP32C's.
 *  arg1 = freq;  i.e. f1, f2, f3
 *  arg2 = board;
 *  arg3 = DSP on board ( neg means all four DSP's on a quad board ).
 * ...repeat above 2 arguments for other boards or individual DSP's.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 
#include "stdioLib.h"
#include "ctype.h"
#include "varargs.h"
#include "ELDRP7.h"
 
unsigned long getaddr();
unsigned int  load8();

int dspstop(va_alist)
va_dcl 

{
  va_list ap;
  unsigned long  bd, frq;
  unsigned char *pio, *temp;
  int i, dsp, dspst, dspen, n, na, status;

 va_start(ap);

    n = va_arg(ap, int);

  for(na = 0; na < n; na++)
    {
	frq = va_arg(ap,int);           /* frequency 1,2,or 3 */
	frq = (frq << 24) + EXTD_BASE;   /* freq # * 10**6 + EXTD_BASE */
	bd = va_arg(ap, int);           /* Board number */
	bd = bd << 20;                  /* Board # * 10**5 = base addr */
	bd += frq;                      /* Full base addr of board */
    dsp = va_arg(ap, int);
    printf("bd=%x dsp=%x\n", bd, dsp);

    if(dsp < 0)
         {
         dspst = 0;
         dspen = 3;
         }
    else
         {
         dspst = dsp;
         dspen = dsp;
         }
    for(i=dspst;i<=dspen;i++)
    {
	pio = (unsigned char *)(bd + (DSPSEL + ((unsigned long)i * DSPOFF)));
        /* Stop DSP */
        *(pio + PCRL) = DMA + AUTO + REGMAP + DSPRESET;
	/* verify DSP is stopped */
	temp = (unsigned char *)(pio + PCRL);
	status = *temp & 0x9f;    /* mask off PDF and PIF */
	if(status != DMA + AUTO + REGMAP + DSPRESET)
	  printf("DSP %d on Board %x Failed to Stop; status = %x \n", i,bd,status);
    }
  }
va_end(ap)
}

