/*
 *	$Id$
 *
 *	Module: GATE_AVG		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  17:11:10  eric
 * Initial revision
 *
 *
 * description:
 *
 *  This module programs the Collator with key parameter flags obtained
 *  from the header file.
 *
 *  arg1 = num_int1
 *  arg2 = num_int2
 *  arg3 = num_int3
 *  arg4 = num_int4
 *  arg5 = chip_avg1
 *  arg6 = chip_avg2
 *  arg7 = chip_avg3
 *  arg8 = chip_avg4
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#ifndef NU_ARCHIT
#include "ELDRP7.h"
#endif
#ifdef NU_ARCHIT
#include "NU_ELDRP7.h"
#endif
#include "coll_dpramdefs.h"
 
int gate_avg(num_int1,num_int2,num_int3,num_int4,chip_avg1,chip_avg2,chip_avg3,chip_avg4)
unsigned int num_int1, num_int2, num_int3, num_int4, chip_avg1, chip_avg2,
             chip_avg3, chip_avg4; 

{
    unsigned long  bd;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status;

/*  Collator  */

    bd = COL0BASE;
    val = num_int1;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_NUM_INT1) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT1;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT1) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_NUM_INT1) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT1;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT1) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = num_int2;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_NUM_INT2) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT2;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT2) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_NUM_INT2) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT2;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT2) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = num_int3;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_NUM_INT3) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT3;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT3) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_NUM_INT3) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT3;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT3) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = num_int4;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_NUM_INT4) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT4;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT4) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_NUM_INT4) >> 16;
    *(pio + PARL) = 0xff & COL_NUM_INT4;
    *(pio + PARH) = (0x00ff00 & COL_NUM_INT4) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = chip_avg1;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG1) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG1;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG1) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG1) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG1;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG1) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = chip_avg2;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG2) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG2;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG2) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG2) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG2;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG2) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = chip_avg3;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG3) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG3;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG3) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG3) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG3;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG3) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = chip_avg4;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG4) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG4;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG4) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_CHIP_AVG4) >> 16;
    *(pio + PARL) = 0xff & COL_CHIP_AVG4;
    *(pio + PARH) = (0x00ff00 & COL_CHIP_AVG4) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

}





