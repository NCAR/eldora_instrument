/*
 *	$Id$
 *
 *	Module: OPFLAGS		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/02/15  23:13:17  eric
 * added support for frequencies 4 and 5, but need
 * to add pick_gate as well. Since this creates an
 * incompatibility w/ the old architecture, this rev will
 * be frozen at this level. Just in case ...
 *
 * Revision 1.1  1992/11/09  22:58:32  eric
 * Initial revision
 *
 *
 *
 * description:
 *
 *  Changes several key parameters in various DSPQC locations.
 *  arg1 = prt_flag;
 *  arg2 = f1_flag;
 *  arg3 = f2_flag;
 *  arg4 = f3_flag;
 *  arg5 = f4_flag;
 *  arg6 = f5_flag;
 *  arg7 = scale;    offset correction to compute dbZ
 *  arg8 = pick_gate
 *  arg9 = noise power;
 *  arg10 = frequency; 1,2, or 3
 *  arg11 = board
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"
#include "dp_ramdefs.h"

extern int Silent;
void opflags(prt_flg, f1_flg, f2_flg, f3_flg, f4_flg, f5_flg, scale, pick_gate, pnoise, frq, bd)
unsigned int prt_flg, f1_flg, f2_flg, f3_flg, f4_flg, f5_flg, pick_gate;
float scale, pnoise; 
unsigned long bd, frq;
{
    
    unsigned long dspc, scale_fac, noise_pwr;
    unsigned char *pio, scr, *temp;
    unsigned int val, i, j, status, parm;
    unsigned long i3e2dsp();

    scale_fac = i3e2dsp(scale);
    if(!Silent)
      printf("scale_fac = %X \n",scale_fac);
    noise_pwr = i3e2dsp(pnoise);
    if(!Silent)
      printf("noise_pwr = %X \n",noise_pwr);

    frq = (frq << 24) + EXTD_BASE;     /* freq # * 10**6 + EXTD_BASE */
    bd = bd << 20;                    /* Board # * 10**5 = base addr */
    bd += frq;                        /* Full base addr of board */
    for(i=0;i<4;i++)

      {
	  val = prt_flg;
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_PRT_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
    
	  /* check that correct values were written */

          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	  val = f1_flg;
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_F1_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
	  
	  /* check that correct values were written */
	  
          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	  val = f2_flg;
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_F2_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
		
	  /* check that correct values were written */
	  
          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	  val = f3_flg;
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_F3_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
    
	  /* check that correct values were written */
	  
          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	  val = f4_flg;
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_F4_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
    
	  /* check that correct values were written */

          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	  val = f5_flg;
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_F5_FLAG;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
    
	  /* check that correct values were written */
	  
          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int)DP_RADAR_CONST;
	  *(pio + PCRL) = AUTO + DMA + REGMAP; /* setup Auto incrementing */
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = scale_fac & 0xFF;
	  *(pio + PDRH) = (scale_fac >> 8) & 0xFF;
	  *(pio + PDRL) = (scale_fac >> 16) & 0xFF;
	  *(pio + PDRH) = (scale_fac >> 24) & 0xFF;
	  /* Test that the scale factor is correct */

          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += (*temp & 0xff)<< 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status += (*temp & 0xff)<< 16;
                  temp = (unsigned char *)(pio + PDRH);
                  status += (*temp & 0xff)<< 24;
                  *(pio + PCRL) = DMA + REGMAP;  /* turn off AUTO increment */
                  j++;
              }
          while(status != scale_fac && j<6);
	  if(status != scale_fac)
	    {
		printf("PIO R/W Error for SCALE_FAC in Board %x, Processor %d \n"
		       , bd,i);
		printf("status = %x \n",status);
	    }

	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int)DP_PNOISE;
	  *(pio + PCRL) = AUTO + DMA + REGMAP; /* setup Auto incrementing */
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = noise_pwr & 0xFF;
	  *(pio + PDRH) = (noise_pwr >> 8) & 0xFF;
	  *(pio + PDRL) = (noise_pwr >> 16) & 0xFF;
	  *(pio + PDRH) = (noise_pwr >> 24) & 0xFF;
	  /* Test that the scale factor is correct */

          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += (*temp & 0xff)<< 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status += (*temp & 0xff)<< 16;
                  temp = (unsigned char *)(pio + PDRH);
                  status += (*temp & 0xff)<< 24;
                  *(pio + PCRL) = DMA + REGMAP;  /* turn off AUTO increment */
                  j++;
              }
          while(status != noise_pwr && j<6);
	  if(status != noise_pwr)
	    {
		printf("PIO R/W Error for NOISE_PWR in Board %x, Processor %d \n"
		       , bd,i);
		printf("status = %x \n",status);
	    }


	  val = (float)(pick_gate)/8.0 + 0.5;  /* ensure pick gate resides in dsp0 */
	  pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	  parm = (unsigned int )DP_PICK_GATE;
	  *(pio + PARE) = (0xff0000 & parm) >> 16;
	  *(pio + PARL) = 0xff & parm;
	  *(pio + PARH) = (0xff00 & parm) >> 8;
	  *(pio + PDRL) = val & 0xFF;
	  *(pio + PDRH) = (val >> 8) & 0xFF;
    
	  /* check that correct values were written */


          j = 0;
          do
              {
                  *(pio + PARE) = (0xff0000 & parm) >> 16;
                  *(pio + PARL) = 0xff & parm;
                  *(pio + PARH) = (0xff00 & parm) >> 8;
                  temp = (unsigned char *)(pio + PDRL);
                  status = *temp & 0xff;
                  temp = (unsigned char *)(pio + PDRH);
                  status += ((*temp & 0xff)<< 8);
                  j++;
              }
          while(status != val && j<6);
	  if(status != val)
	    printf("PIO R/W ERROR in Board %x, Processor %d \n",bd,i);

      }
} 






