/*
 *	$Id$
 *
 *	Module: V_SCALE		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  22:58:32  eric
 * Initial revision
 *
 *
 *
 * description:
 *
 *  Changes several key parameters in various DSPQC locations.
 *  arg1 = frequency in GHz;
 *  arg2 = prf in Hz;
 *  arg3 = frequency; 1,2, or 3
 *  arg4 = board
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"
#include "dp_ramdefs.h"
#define Fmin 9.30
#define PI 3.141592654
unsigned long i3e2dsp();
int v_scale(freq_act,frq,bd)
float freq_act;
unsigned long bd,frq;

{
    unsigned long dspc, dpv_scale, status;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, parm;
    float vscale;

    vscale = (127/PI)*(Fmin / freq_act);

    /* Convert to DSP32C floating point format */

    dpv_scale = i3e2dsp(vscale);
    frq = (frq << 24) + EXTD_BASE;    /* freq # * 10**6 + EXTD_BASE */
    bd = bd << 20;                    /* Board # * 10**5 = base addr */
    bd += frq;                        /* Full base addr of board */
    for(i=0;i<4;i++)
      {
	  val = (unsigned int )(dpv_scale & 0xffff);
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_VSCALE;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  val = (unsigned int )((dpv_scale >> 16) & 0xffff);
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  /* check that correct values were written */
	  
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  temp = (unsigned char *)(pio + PDRL);
	  status = *temp & 0xff;
	  temp = (unsigned char *)(pio + PDRH);
	  status += ((*temp & 0xff)<< 8);
	  temp = (unsigned char *)(pio + PDRL);
	  status += ((*temp & 0xff) << 16);
	  temp = (unsigned char *)(pio + PDRH);
	  status += ((*temp & 0xff)<< 24);
	  if(status != dpv_scale)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);
		
      }
    
}















