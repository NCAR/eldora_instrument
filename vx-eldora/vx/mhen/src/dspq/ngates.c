/*
 *	$Id$
 *
 *	Module:	NGATES	 
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
 * description:
 *
 *  Loads the appropriate number of gates into PPP, DP, or Collator.
 *  arg1 = number of gates
 *  arg2 = number of boards to program
 *  arg3 = frequency; 1,2, or 3
 *  arg4 = board      
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 
#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "stdarg.h"
#ifndef NU_ARCHIT
#include "ELDRP7.h"
#endif
#ifdef NU_ARCHIT
#include "NU_ELDRP7.h"
#endif
extern int Silent;
unsigned long getaddr();
unsigned int  load4();
unsigned int num_gates; 

int ngates(int gates)


{
  va_list ap;
  unsigned long  bd, frq, dspc;
  unsigned char *pio, scr, *temp;
  unsigned int valc, vald, i, status;
  int n, na;

  va_start(ap,gates);
  num_gates = gates;    /* Read in number of gates */
  valc = num_gates;
 
  n = va_arg(ap, int);
  for(na = 0; na < n; na++)
    {
	frq = va_arg(ap, int);          /* freq 1,2, or 3 */
	frq = (frq << 24) + EXTD_BASE;   /* freq # * 10**6 + EXTD_BASE*/
	bd = va_arg(ap, int);           /* board number */
	switch (bd)
	  {
	    case 3:
	      vald = valc/8;
	      valc = vald*8;
	      if(!Silent)
		printf("%5d gate count to Doppler Processors\n",vald);
	      break;
	    default:
	      vald = valc/4;
	      valc = vald*4;
	      if(!Silent)
		printf("%5d gate count to Pulse Pair Processors\n",vald);
	      break;
	  }
	if(!Silent)
	  printf("%5d gate count to Collator\n",valc);
	bd = bd << 20;                  /* Board # * 10**5 = base addr */
	bd += frq;                      /* Full base addr. of board */
	for(i=0;i<4;i++)
	  {
	      pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
              *(pio + PCRL) = 0x1A;           /* set up DMA and AUTO */
	      if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
	      *(pio + PARE) = 0x0;
	      *(pio + PARL) = 0x50;
	      *(pio + PARH) = 0;
	      *(pio + PDRL) = vald & 0xFF;
	      *(pio + PDRH) = (vald >> 8) & 0xFF;
	      /* Test that number of gates written is correct */
	      *(pio + PARE) = 0x0;
	      *(pio + PARL) = 0x50;
	      *(pio + PARH) = 0;
	      temp = (unsigned char *)(pio + PDRL);
	      status = *temp & 0xff;
	      temp = (unsigned char *)(pio + PDRH);
	      status += (*temp & 0xff)<< 8;
	      if(status != vald)
		{
		    if(!Silent)
		      printf("PIO R/W Error in Board %x, Processor %d \n", bd,i);
		}
	      else
		{
		    if(!Silent)
		      printf("PIO loaded successfully in Board %x, Processor %d \n",bd,i);
		}
          }
    }
  va_end(ap);

/*   Now for the Collator */

  bd = COL0BASE;        /* Base address of Collator */
  pio = (unsigned char *)(bd +  DSPSEL);
  if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
  *(pio + PARE) = 0x0;
  *(pio + PARL) = 0x50;
  *(pio + PARH) = 0x0;
  *(pio + PDRL) = valc & 0xFF;
  *(pio + PDRH) = (valc >> 8) & 0xFF;
  /* Test that number of gates written is correct */
  *(pio + PARE) = 0x0;
  *(pio + PARL) = 0x50;
  *(pio + PARH) = 0;
  temp = (unsigned char *)(pio + PDRL);
  status = *temp & 0xff;
  temp = (unsigned char *)(pio + PDRH);
  status += ((*temp & 0xff)<<8);
  if(status != valc)
    {
	if(!Silent)
	  printf("PIO R/W Error in Collator Processor 0 \n");
    }
  else
    {
	if(!Silent)
	  printf("PIO loaded successfully in Collator Processor 0 \n");
    }

}



























