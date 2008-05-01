/*  
  Poke HC11 reset vector into ECB MASTER DPRAM
  
  NOTE: actually sets all interrupt vectors to the addr.
  
  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

void pokevec(addr)
     
     unsigned short addr;
{
  unsigned char addrh,addrl,*rstvec;
  
  rstvec = (unsigned char *) (MASTERBASE + 0xFFC0);
  
  addrh = addr >> 8;
  addrl = addr & 0xff;
  
  printf("Vector, MSB, LSB ==> %4x, %2x, %2x\n",addr,addrh,addrl);

  while ( rstvec < (unsigned char *) (MASTERBASE + 0xffff) )
    {  
      *rstvec = addrh;
      rstvec++;
      *rstvec = addrl;
      rstvec++;
    }
  printf("...vector 0x%4x poked into address ffc0-fffe\n");
}






