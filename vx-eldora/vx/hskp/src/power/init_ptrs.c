/*
 *	$Id$
 *
 *	Module:	init_ptrs.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void init_ptrs()
{

/***************  GPIB REGISTER POINTERS  *********************************/

g1ism0 = (unsigned char *)(GPIBCHAN1 + INT_STAT_REG0);
g1ism1 = (unsigned char *)(GPIBCHAN1 + INT_STAT_REG1);
g1acr = (unsigned char *)(GPIBCHAN1 + AUX_CMND_REG);
g1adr = (unsigned char *)(GPIBCHAN1 + ADDRESS_REG);
g1dido = (unsigned char *)(GPIBCHAN1 + DATA_IN_OUT_REG);
g2ism0 = (unsigned char *)(GPIBCHAN2 + INT_STAT_REG0);
g2ism1 = (unsigned char *)(GPIBCHAN2 + INT_STAT_REG1);
g2acr = (unsigned char *)(GPIBCHAN2 + AUX_CMND_REG);
g2adr = (unsigned char *)(GPIBCHAN2 + ADDRESS_REG);
g2dido = (unsigned char *)(GPIBCHAN2 + DATA_IN_OUT_REG);

/***************  INIT DMA REGISTER POINTERS **********************/

d0csr = (unsigned char *)(DMACHAN0 + CHAN_STAT_REG);
d0dcr = (unsigned char *)(DMACHAN0 + DEV_CNTRL_REG);
d0ocr = (unsigned char *)(DMACHAN0 + OP_CNTRL_REG);
d0scr = (unsigned char *)(DMACHAN0 + SEQ_CNTRL_REG);
d0ccr = (unsigned char *)(DMACHAN0 + CHAN_CNTRL_REG);
d0mtc = (unsigned short *)(DMACHAN0 + MEM_XFER_CNTR);
d0bar1 = (unsigned short *)(DMACHAN0 + BASE_ADDR_REG1);
d0bar2 = (unsigned short *)(DMACHAN0 + BASE_ADDR_REG2);
d0mar1 = (unsigned short *)(DMACHAN0 + MEM_ADDR_REG1);
d0mar2 = (unsigned short *)(DMACHAN0 + MEM_ADDR_REG2);
d0btc = (unsigned short *)(DMACHAN0 + BASE_XFER_CNTR);
d0niv = (unsigned char *)(DMACHAN0 + NORM_INT_VECT);
d0eiv = (unsigned char *)(DMACHAN0 + ERR_INT_VECT);
d0cpr = (unsigned char *)(DMACHAN0 + CHAN_PRTY_REG);
d0mfc = (unsigned char *)(DMACHAN0 + MEM_FUNC_CODE);
d0bfc = (unsigned char *)(DMACHAN0 + BASE_FUNC_CODE);
gcr = (unsigned char *)(DMACHAN0 + GEN_CNTRL_REG);
d1csr = (unsigned char *)(DMACHAN1 + CHAN_STAT_REG);
d1dcr = (unsigned char *)(DMACHAN1 + DEV_CNTRL_REG);
d1ocr = (unsigned char *)(DMACHAN1 + OP_CNTRL_REG);
d1scr = (unsigned char *)(DMACHAN1 + SEQ_CNTRL_REG);
d1ccr = (unsigned char *)(DMACHAN1 + CHAN_CNTRL_REG);
d1mtc = (unsigned short *)(DMACHAN1 + MEM_XFER_CNTR);
d1bar1 = (unsigned short *)(DMACHAN1 + BASE_ADDR_REG1);
d1bar2 = (unsigned short *)(DMACHAN1 + BASE_ADDR_REG2);
d1mar1 = (unsigned short *)(DMACHAN1 + MEM_ADDR_REG1);
d1mar2 = (unsigned short *)(DMACHAN1 + MEM_ADDR_REG2);
d1btc = (unsigned short *)(DMACHAN1 + BASE_XFER_CNTR);
d1niv = (unsigned char *)(DMACHAN1 + NORM_INT_VECT);
d1eiv = (unsigned char *)(DMACHAN1 + ERR_INT_VECT);
d1cpr = (unsigned char *)(DMACHAN1 + CHAN_PRTY_REG);
d1mfc = (unsigned char *)(DMACHAN1 + MEM_FUNC_CODE);
d1bfc = (unsigned char *)(DMACHAN1 + BASE_FUNC_CODE);
}




