/*
 *	$Id$
 *
 *	Module: COLL_OPFLAGS		 
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
 *  This module programs the Collator with key parameter flags obtained
 *  from the header file.
 *
 *  arg1 = prt_flag
 *  arg2 = f1_flag
 *  arg3 = f2_flag
 *  arg4 = f3_flag
 *  arg5 = coll_pick_gate
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"

int coll_opflags(prt_flg, f1_flg, f2_flg, f3_flg, pick_gate) 
unsigned int prt_flg, f1_flg, f2_flg, f3_flg, pick_gate; 

{
    unsigned long  bd;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status;

/*  Collator  */

    bd = COL0BASE;
    val = prt_flg;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_PRT_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_PRT_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_PRT_FLAG) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_PRT_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_PRT_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_PRT_FLAG) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = f1_flg;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_F1_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_F1_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_F1_FLAG) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_F1_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_F1_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_F1_FLAG) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = f2_flg;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_F2_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_F2_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_F2_FLAG) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_F2_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_F2_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_F2_FLAG) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = f3_flg;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_F3_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_F3_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_F3_FLAG) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_F3_FLAG) >> 16;
    *(pio + PARL) = 0xff & COL_F3_FLAG;
    *(pio + PARH) = (0x00ff00 & COL_F3_FLAG) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

    val = pick_gate;
    pio = (unsigned char *)(bd +  DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PARE) = (0xff0000 & COL_PICK_GATE) >> 16;
    *(pio + PARL) = 0xff & COL_PICK_GATE;
    *(pio + PARH) = (0x00ff00 & COL_PICK_GATE) >> 8;
    *(pio + PDRL) = val & 0xFF;
    *(pio + PDRH) = (val >> 8) & 0xFF;
    
    /* check that correct values were written */
    
    *(pio + PARE) = (0xff0000 & COL_PICK_GATE) >> 16;
    *(pio + PARL) = 0xff & COL_PICK_GATE;
    *(pio + PARH) = (0x00ff00 & COL_PICK_GATE) >> 8;
    temp = (unsigned char *)(pio + PDRL);
    status = *temp & 0xff;
    temp = (unsigned char *)(pio + PDRH);
    status += ((*temp & 0xff)<< 8);
    if(status != val)
      printf("PIO R/W ERROR in Collator, Processor 0 \n");

}



