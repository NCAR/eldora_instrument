/*
 *	$Id$
 *
 *	Module: DP_INTR		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 *
 * description:
 *
 *  Tests for proper interrupt operation of individual DSP32C's on Doppler Processor.
 *  arg1 = frequency; 1,2, or 3 of DP
 *  arg2 = board
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"
#include "dp_ramdefs.h"
  
void dp_intr(frq,bd)
unsigned long bd,frq;
{
    unsigned dspc;
    unsigned char *pio, scr, *temp, *led;
    unsigned int val, i, status, parm;

    frq = (frq << 24) + EXTD_BASE;     /* freq # * 10**6 + EXTD_BASE */
    bd = bd << 20;                    /* Board # * 10**5 = base addr */
    bd += frq;                        /* Full base addr of board */
    
    pio = (unsigned char *)(bd + DSPSEL);  /* only need to check DSP0 */
    *(pio + PCRL) = AUTO + DMA + ENI + REGMAP + RUN;    /* set up DMA and AUTO */
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    parm = (unsigned int )DP_INT_STATUS;
    *(pio + PARE) = (0xff0000 & parm) >> 16;
    *(pio + PARL) = 0xff & parm;
    *(pio + PARH) = (0xff00 & parm) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    switch (status)
      {
	case 0:
	  printf("DP INTERRUPTS OK \n");
	  break;
	case 1:
	  printf("DP INTERRUPT ERROR: Interrupted w/o data xfer. \n");
	  break;
	case 2:
	  printf("DP INTERRUPT ERROR:  Data xfer w/o interrupt \n");
	  break;
      }
    /* Reset out of interrupt flag */

    val = 0;
    pio = (unsigned char *)(bd + DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    parm = (unsigned int )DP_INT_STATUS;
    *(pio + PARE) = (0xff0000 & parm) >> 16;
    *(pio + PARL) = 0xff & parm;
    *(pio + PARH) = (0xff00 & parm) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    *(pio + PCRL) = REGMAP + RUN + ENI;   /* Return PCR to normal operation */
    
}



