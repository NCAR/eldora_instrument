/*
 *	$Id$
 *
 *	Module: COL_SYNC		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/02/15  23:34:59  eric
 * modified frequency 1 to work correctly by enabling DMA in PCR.
 * This version will reflect the code that functioned in the old
 * RP7 architecture. Since there are no longer 3 Doppler Processors,
 * This version is no longer applicaple to the new architecture.
 *
 * Revision 1.1  1992/11/09  17:11:14  eric
 * Initial revision
 *
 *
 * description:
 *
 *  Tests for synchronization of F1,F2 and F3 Doppler Processors.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#define  RP_EOF  0x7FFFFE96   /* = 0x7fffff integer */  
int col_sync()
{
    unsigned dspc;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status, parm, flag;
    unsigned long bd;

    bd = COL0BASE;

    /* Read Collator's EOF */
    
    flag = 0;
    pio = (unsigned char *)(bd + DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PCRL) = AUTO + DMA + ENI + REGMAP + RUN;    /* set up DMA and AUTO */
    parm = (unsigned int )COL_F1_SYNC;
    *(pio + PARE) = (0xff0000 & parm) >> 16;
    *(pio + PARL) = 0xff & parm;
    *(pio + PARH) = (0xff00 & parm) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    temp = (unsigned char *)(pio + PDRL);
    status += (*temp & 0xff) << 16;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 24);
    *(pio + PCRL) = REGMAP + RUN + ENI; /* disable Auto incrementing */
    if(status != RP_EOF)
      {
	  flag |= 0x1;
	  printf("COLLATOR SYNC ERROR\n" );
	  printf("Status = %8x \n",status);
      }
    *(pio + PCRL) = REGMAP + RUN + ENI;   /* Return PCR to normal operation */
    return(flag);
}


