/*  
  Repeatedly write/read test ECB MASTER DPRAM.
  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

unsigned short dptest(bank,cycles)

     unsigned char bank;
     unsigned int cycles;

{
  unsigned char *dpram,*ctl,ledval;
  unsigned short value;
  unsigned int i,offset,nerr;
  
  if (bank < 0 | bank > 3 | cycles == 0)
    {
      printf("usage:  dptest bank,cycles\n");
      printf("               bank must be between 0 and 3.\n");
      printf("                    cycles must be greater than 0.\n");
      return(1);
    }

  nerr = ledval = 0;

  dpram = (unsigned char *)(MASTERBASE) + (bank * 0x4000);
  ctl = (unsigned char *)(MASTERBASE + MASTERCTL);
  for (i=0; i<cycles; i++)
    {
      dpram = (unsigned char *)(MASTERBASE) + (bank * 0x4000);
      for (offset=0; offset<0x4000; offset++, dpram++)
	{
	  if (((offset/100)*100) == offset)
	    *ctl = ledval = ledval ^ 0x20;     /* toggle LED */
	  if (bank==3 & offset>=0x3000 & offset<=0x37ff) 
	    continue;   /* skip Memory-mapped IO section */
	  for (value = 0; value <= 0xff; value++)
	    {
	      *dpram = value;
	      if (*dpram != value)
		{
		  nerr++;
		  printf("readback error at location %4x, value of %3x\n",dpram,value);
		}
	    }
	}
    }
  return (nerr);
}
