#include "NU_ELDRP7.h"
#include "vxWorks.h"

/* 
this program will display collator memory to tty
of length specified in long words from start address.
*/
void coll_dmp(unsigned long start_addr,int length)
{
int i,j;
unsigned int *dump_ptr;
unsigned long addr;

addr = start_addr;
dump_ptr = (float *)(start_addr);
j=3;
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
      printf("%8x  ",*dump_ptr);
      dump_ptr++;
  }
}
