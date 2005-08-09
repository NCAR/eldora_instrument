#include "NU_ELDRP7.h"
#include "vxWorks.h"

/* 
this program will display DSPQC memory to tty
of length specified in long words from start address.
*/
void quad_dmp(unsigned long freq, unsigned long bd, short dsp, unsigned long start_addr,int length)
{
int i,j;
unsigned int status;
unsigned long addr;
unsigned char *pio,*temp,scr;

freq = (freq << 24) + EXTD_BASE;
bd = bd << 20;
bd += freq;
pio = (unsigned char *)(bd + (DSPSEL + dsp * DSPOFF));
*(pio + PCRL) = 0x1A;           /* set up DMA and AUTO */
if(*(pio + PCRL) != 0)scr = *(pio + PDRH);

addr = start_addr;
j=3;
*(pio + PARE) = (addr >> 16) & 0xff;
*(pio + PARL) = addr & 0xff;
*(pio + PARH) = (addr >> 8) & 0xff;
for(i=0; i<length; i++)
  {
      if(j == 3)
	{
	    printf("\n %x:   ",addr);
	    addr += 0x10;
	    j = 0;
	}
      else
	    j++;
      temp = (unsigned char *)(pio + PDRL);
      status = *temp & 0xff;
      temp = (unsigned char *)(pio + PDRH);
      status += (*temp & 0xff)<< 8;
      temp = (unsigned char *)(pio + PDRL);
      status += (*temp & 0xff)<< 16;
      temp = (unsigned char *)(pio + PDRH);
      status += (*temp & 0xff)<< 24;
      printf("%8x  ",status);

  }
}

