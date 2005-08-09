/*
 *	$Id$
 *
 *	Module: DP_SYNC		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/11/16  20:54:17  eric
 * Added case to print out EOF regardless of whether DSP
 * is out of sync.
 *
 * Revision 1.1  1992/11/09  22:58:34  eric
 * Initial revision
 *
 *
 *
 * description:
 *
 *  Tests for synchronization of individual DSP32C's on Doppler Processor.
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
  
int dp_sync(frq,bd)
unsigned long bd,frq;
{
    unsigned dspc;
    unsigned char *pio, scr, *temp, *led;
    unsigned int val, i, status, parm, flag;

    flag = 0;
    frq = (frq << 24) + EXTD_BASE;     /* freq # * 10**6 + EXTD_BASE */
    bd = bd << 20;                    /* Board # * 10**5 = base addr */
    bd += frq;                        /* Full base addr of board */
    for(i=0;i<4;i++)
      {
	  /* Check if out of sync flag has been set */

	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  *(pio + PCRL) = AUTO + DMA + ENI + REGMAP + RUN;    /* set up DMA and AUTO */
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_SYNC_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  temp = (unsigned char *)(pio + PDRL);
	  status = *temp & 0xff;
	  temp = (unsigned char *)(pio + PDRH);
	  status += ((*temp & 0xff)<< 8);
	  if(status != 0)
	    {
		flag |= 0x1;
		printf("DP OUT OF SYNC ERROR in Board %x, Processor %d \n",bd,i);
		/* Turn off LED on DP */

		led = (unsigned char*)(bd + DLEDOFF);
		*led = 0x0;

		/* read eof received */
		pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		parm = (unsigned int )DP_EOF_RCVD;
		*(pio + PARE) = (0xff0000 & parm) >> 16;
		*(pio + PARL) = 0xff & parm;
		*(pio + PARH) = (0xff00 & parm) >> 8;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		*(pio + PARE) = (0xff0000 & (parm + 2)) >> 16;
		*(pio + PARL) = 0xff & (parm + 2);
		*(pio + PARH) = (0xff00 & (parm + 2)) >> 8;
		temp = (unsigned char *)(pio + PDRL);
		status += (*temp & 0xff) << 16;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 24);
		printf("EOF received = %8X \n",status);
		
		/* Reset out of sync flag */
		val = 0;
		pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		parm = (unsigned int )DP_SYNC_FLAG;
		*(pio + PARE) = (0xff0000 & parm) >> 16;
		*(pio + PARL) = 0xff & parm;
		*(pio + PARH) = (0xff00 & parm) >> 8;
		*(pio + PDRL) = val & 0xFF;
		*(pio + PDRH) = (val >> 8) & 0xFF;
			     
	    }
	  else
	    {
		/* read eof received */
		pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		parm = (unsigned int )DP_EOF_RCVD;
		*(pio + PARE) = (0xff0000 & parm) >> 16;
		*(pio + PARL) = 0xff & parm;
		*(pio + PARH) = (0xff00 & parm) >> 8;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		*(pio + PARE) = (0xff0000 & (parm + 2)) >> 16;
		*(pio + PARL) = 0xff & (parm + 2);
		*(pio + PARH) = (0xff00 & (parm + 2)) >> 8;
		temp = (unsigned char *)(pio + PDRL);
		status += (*temp & 0xff) << 16;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 24);
		printf("STATUS OK Board = %x, Processor = %d \n",bd,i);
		printf("EOF received = %8X \n",status);
	    }
 
	  *(pio + PCRL) = REGMAP + RUN + ENI;   /* Return PCR to normal operation */
      }
    return(flag);
}



