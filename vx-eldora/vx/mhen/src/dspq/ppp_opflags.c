/*
 *	$Id$
 *
 *	Module:	PPP_OPFLAGS	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 *
 *
 * description:
 *
 *  Loads the gates variable into four DSP's and the collator.
 *  arg1 = Pulse Pair Processor picked gate (for time series)
 *  arg2 = Frequency on flag; 1 = ON, 0 = OFF
 *  arg3 = Input Scale Factor
 *  arg4 = DC removal flag
 *  arg5 = frequency
 *  arg6 = board
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 
#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "ELDRP7.h"

void ppp_opflags(pick_gate,freq_on,prt_flag,scale,dc_flag,frq,bd)
float scale;
unsigned long frq, bd;
unsigned int pick_gate, freq_on, dc_flag, prt_flag;

{
  unsigned long  dspc, scale_fac;
  unsigned char *pio, scr, *temp;
  unsigned int  i, status;

unsigned long i3e2dsp();

  printf("freq_on = %d\n",freq_on);
  printf("scale = %f\n",scale);
  scale_fac = i3e2dsp(scale);
  printf("scale_fac = %X \n",scale_fac);
  pick_gate = (float)(pick_gate)/4.0 + 0.5;

  frq = (frq << 24) + EXTD_BASE;   /* freq # * 10**6 + EXTD_BASE*/
  bd = bd << 20;                  /* Board # * 10**5 = base addr */
  bd += frq;                      /* Full base addr. of board */
  for(i=0;i<4;i++)
    {
        /* Set Time Series Gate to be Acquired */

	pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x6C;
	*(pio + PARH) = 0;
	*(pio + PDRL) = pick_gate & 0xFF;
	*(pio + PDRH) = (pick_gate >> 8) & 0xFF;
	/* Test that the time series gate written is correct */
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x6C;
	*(pio + PARH) = 0;
	temp = (unsigned char *)(pio + PDRL);
	status = *temp & 0xff;
	temp = (unsigned char *)(pio + PDRH);
	status += (*temp & 0xff)<< 8;
	if(status != pick_gate)
	  printf("PIO R/W Error for PICK_GATE in Board %x, Processor %d \n", bd,i);

	/* Set Appropriate FREQ_ON Status */

	pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x14;
	*(pio + PARH) = 1;
	*(pio + PDRL) = freq_on & 0xFF;
	*(pio + PDRH) = (freq_on >> 8) & 0xFF;
	/* Test that the freq_on status is correct */
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x14;
	*(pio + PARH) = 1;
	temp = (unsigned char *)(pio + PDRL);
	status = *temp & 0xff;
	temp = (unsigned char *)(pio + PDRH);
	status += (*temp & 0xff)<< 8;
	if(status != freq_on)
	  printf("PIO R/W Error for FREQ_ON in Board %x, Processor %d \n", bd,i);

	/* Set Appropriate PRT_FLAG Status */

	pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x12;
	*(pio + PARH) = 1;
	*(pio + PDRL) = prt_flag & 0xFF;
	*(pio + PDRH) = (prt_flag >> 8) & 0xFF;
	/* Test that the prt_flag status is correct */
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x12;
	*(pio + PARH) = 1;
	temp = (unsigned char *)(pio + PDRL);
	status = *temp & 0xff;
	temp = (unsigned char *)(pio + PDRH);
	status += (*temp & 0xff)<< 8;
	if(status != prt_flag)
	  printf("PIO R/W Error for PRT_FLAG in Board %x, Processor %d \n", bd,i);
	
	/* Set scale_fac Value to Scale Input Data Correctly */

	pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	*(pio + PCRL) = 0x1a; /* setup Auto incrementing */
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x18;
	*(pio + PARH) = 1;
	*(pio + PDRL) = scale_fac & 0xFF;
	*(pio + PDRH) = (scale_fac >> 8) & 0xFF;
	*(pio + PDRL) = (scale_fac >> 16) & 0xFF;
	*(pio + PDRH) = (scale_fac >> 24) & 0xFF;
	/* Test that the scale factor is correct */
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x18;
	*(pio + PARH) = 1;
	temp = (unsigned char *)(pio + PDRL);
	status = *temp & 0xff;
	temp = (unsigned char *)(pio + PDRH);
	status += (*temp & 0xff)<< 8;
	temp = (unsigned char *)(pio + PDRL);
	status += (*temp & 0xff)<< 16;
	temp = (unsigned char *)(pio + PDRH);
	status += (*temp & 0xff)<< 24;
	if(status != scale_fac)
	  {
	      printf("PIO R/W Error for SCALE_FAC in Board %x, Processor %d \n", bd,i);
	      printf("status = %x \n",status);
	  }
	/* Set Appropriate DC_FLAG Status */

	pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x22;
	*(pio + PARH) = 1;
	*(pio + PDRL) = dc_flag & 0xFF;
	*(pio + PDRH) = (dc_flag >> 8) & 0xFF;
	/* Test that the dc_flag status is correct */
	*(pio + PARE) = 0x0;
	*(pio + PARL) = 0x22;
	*(pio + PARH) = 1;
	temp = (unsigned char *)(pio + PDRL);
	status = *temp & 0xff;
	temp = (unsigned char *)(pio + PDRH);
	status += (*temp & 0xff)<< 8;
	if(status != dc_flag)
	  printf("PIO R/W Error for DC_FLAG in Board %x, Processor %d \n", bd,i);
	
	if(dc_flag)   /* if doing DC Removal Zero out i and q offsets */
	  {
	      pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
	      if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	      *(pio + PCRL) = 0x1a; /* setup Auto incrementing */
	      *(pio + PARE) = 0x0;
	      *(pio + PARL) = 0x24;
	      *(pio + PARH) = 0x1;
	      *(pio + PDRL) = 0x0;
	      *(pio + PDRH) = 0x0;
	      *(pio + PDRL) = 0x0;
	      *(pio + PDRH) = 0x0;
	      *(pio + PDRL) = 0x0;
	      *(pio + PDRH) = 0x0;
	      *(pio + PDRL) = 0x0;
	      *(pio + PDRH) = 0x0;

	/* Test that the i and q offsets are now Zero */
	      *(pio + PARE) = 0x0;
	      *(pio + PARL) = 0x24;
	      *(pio + PARH) = 0x1;
	      temp = (unsigned char *)(pio + PDRL);
	      status = *temp & 0xff;
	      temp = (unsigned char *)(pio + PDRH);
	      status += (*temp & 0xff)<< 8;
	      temp = (unsigned char *)(pio + PDRL);
	      status += (*temp & 0xff)<< 16;
	      temp = (unsigned char *)(pio + PDRH);
	      status += (*temp & 0xff)<< 24;
	      if(status)
		{
		    printf("PIO R/W Error for I_OFF in Board %x, Processor %d \n", bd,i);
		    printf("status = %x \n",status);
		}
	      temp = (unsigned char *)(pio + PDRL);
	      status = *temp & 0xff;
	      temp = (unsigned char *)(pio + PDRH);
	      status += (*temp & 0xff)<< 8;
	      temp = (unsigned char *)(pio + PDRL);
	      status += (*temp & 0xff)<< 16;
	      temp = (unsigned char *)(pio + PDRH);
	      status += (*temp & 0xff)<< 24;
	      if(status)
		{
		    printf("PIO R/W Error for Q_OFF in Board %x, Processor %d \n", bd,i);
		    printf("status = %x \n",status);
		}
	      
	  }
    }
}



























