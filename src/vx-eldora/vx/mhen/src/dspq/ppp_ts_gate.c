/*
 *	$Id$
 *
 *	Module:	PPP_TS_GATE	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 *
 *
 * description:
 *
 *  Loads the time series gate variable into four PPP DSP's.
 *  arg1 = Pulse Pair Processor picked gate (for time series)
 *  arg5 = frequency
 *  arg6 = board
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 
#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"

extern int Silent;
void ppp_ts_gate(pick_gate,frq,bd)
unsigned long frq, bd;
unsigned int pick_gate;

{
  unsigned long  dspc;
  unsigned char *pio, scr, *temp;
  unsigned int  i, j, status;

  pick_gate = (float)(pick_gate)/4.0 + 0.5;

  frq = (frq << 24) + EXTD_BASE;   /* freq # * 10**6 + EXTD_BASE*/
  bd = bd << 20;                  /* Board # * 10**5 = base addr */
  bd += frq;                      /* Full base addr. of board */
  for(i=0;i<4;i++)
    {
        /* Set Time Series Gate to be Acquired */

	pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	/* Set up DMA in DSP32C */
	*(pio + PCRL) = REGMAP + RUN + ENI + DMA;
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x6C;
	*(pio + PARH) = 0;
	*(pio + PDRL) = pick_gate & 0xFF;
	*(pio + PDRH) = (pick_gate >> 8) & 0xFF;
	/* Test that the time series gate written is correct */
        j = 0;
        do
            {
            *(pio + PARE) = 0x0;
            *(pio + PARL) = 0x6C;
            *(pio + PARH) = 0;
            temp = (unsigned char *)(pio + PDRL);
            status = *temp & 0xff;
            temp = (unsigned char *)(pio + PDRH);
            status += (*temp & 0xff)<< 8;
            j++;
            }
        while (status != pick_gate && j<6);
             
	if(status != pick_gate)
	  printf("PIO R/W Error for PICK_GATE in Board %x, Processor %d \n", bd,i);
	/* Clear DMA in DSP32C */
         *(pio + PCRL) = REGMAP + RUN + ENI;
    }
}



























