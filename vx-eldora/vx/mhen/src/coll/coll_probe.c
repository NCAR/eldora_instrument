/*
 *	$Id$
 *
 *	Module: COLL_PROBE		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * 
 *
 * description:
 *
 *  Reads several key COLLATOR locations.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "varargs.h"
#include "ctype.h"
#include "ELDRP7.h"
 
unsigned long getaddr();
unsigned int  load4();
unsigned int num_samples; 
coll_probe()
{
    unsigned long  bd, frq, dspc;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status;
    int n, na;
    
    bd = COL0BASE;
    pio = (unsigned char *)(bd + (DSP0SEL));
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
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    printf("*num_int1 = %x \n",status);
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    printf("*num_int2 = %x \n",status);
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    printf("*num_int3 = %x \n",status);
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    printf("*num_int4 = %x \n",status);
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
    *(pio + PARL) = 0xb0;
    *(pio + PARH) = 0x9;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    printf("*dp_tgates = %x \n",status);
    *(pio + PARE) = 0x0;
    *(pio + PARL) = 0xba;
    *(pio + PARH) = 0x9;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    printf("*tnum_param = %x \n",status);
}



