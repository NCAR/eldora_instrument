/*  
  Repeatedly write/read test all banks of ECB MASTER DPRAM.
  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

alldptest(cycles)
     
     unsigned int cycles;
     
{
  unsigned char *vmectl;
  unsigned int i,cumnerr;
  
  vmectl = (unsigned char *)(MASTERBASE + MASTERCTL);
  cumnerr = 0;
  
  if (cycles == 0)
    {
      printf("usage:  alldptest cycles\n");
      printf("                  cycles must be greater than 0.\n");
      return(1);
    }
  
  printf("\n\n             ECB MASTER Dual-Port RAM test\n\n");
  
  for (i=0; i<cycles; i++)
    {
      *vmectl = 0x00;
      cumnerr += dptest(0,1);
      *vmectl = 0x20;
      cumnerr += dptest(1,1);
      *vmectl = 0x00;
      cumnerr += dptest(2,1);
      *vmectl = 0x20;
      cumnerr += dptest(3,1);
      printf("alldptest: Cycle %d complete, %d cumulative errors    \r",i,cumnerr);
    }
  printf("\n\nalldptest: DPRAM memory test complete.  %d total errors.\n\n",cumnerr);
}
