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
  
void col_sync()
{
    unsigned dspc;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, status, parm;
    unsigned long bd;

    bd = COL0BASE;

    /* Check if F1 out of sync flag has been set */

    pio = (unsigned char *)(bd + DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    *(pio + PCRL) = 0x1b; /* setup Auto incrementing */
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
    if(status)
      {
	  printf("COLLATOR SYNC ERROR CAUSED BY F1 Doppler Processor \n" );
	  printf("Status = %8x \n",status);
	  /* Reset out of F1_SYNC flag */
	  val = 0;
	  pio = (unsigned char *)(bd + DSPSEL);
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )COL_F1_SYNC;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  *(pio + PARE) = (0xff0000 & parm + 2) >> 16;
	  *(pio + PARL) = 0xff & parm + 2;
	  *(pio + PARH) = (0xff00 & parm + 2) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  
      }
    /* Check if F2 out of sync flag has been set */

    pio = (unsigned char *)(bd + DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    parm = (unsigned int )COL_F2_SYNC;
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
    if(status)
      {
	  printf("COLLATOR SYNC ERROR CAUSED BY F2 Doppler Processor \n" );
	  printf("Status = %8x \n",status);		
	  /* Reset out of F2_SYNC flag */
	  val = 0;
	  pio = (unsigned char *)(bd + DSPSEL);
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )COL_F2_SYNC;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  *(pio + PARE) = (0xff0000 & parm + 2) >> 16;
	  *(pio + PARL) = 0xff & parm + 2;
	  *(pio + PARH) = (0xff00 & parm + 2) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  
      }
    /* Check if F3 out of sync flag has been set */

    pio = (unsigned char *)(bd + DSPSEL);
    if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
    parm = (unsigned int )COL_F3_SYNC;
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
    if(status)
      {
	  printf("COLLATOR SYNC ERROR CAUSED BY F3 Doppler Processor \n" );
	  printf("Status = %8x \n",status);		
	  /* Reset out of F3_SYNC flag */
	  val = 0;
	  pio = (unsigned char *)(bd + DSPSEL);
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )COL_F3_SYNC;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  *(pio + PARE) = (0xff0000 & parm + 2) >> 16;
	  *(pio + PARL) = 0xff & parm + 2;
	  *(pio + PARH) = (0xff00 & parm + 2) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  
      }
 
    
    
}


