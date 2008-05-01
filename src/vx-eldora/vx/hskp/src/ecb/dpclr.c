/*  
  dpclr:  Write zeroes to all locations of ECB MASTER DPRAM
  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

void dpclr()
{
  unsigned char *loc;
  
  loc = (unsigned char *) (MASTERBASE + 0x0000);
  
  while ( loc < (unsigned char *) (MASTERBASE + 0x10000) )
    if ((loc < (unsigned char *)(MASTERBASE + 0xf000)) || (loc > (unsigned char *)(MASTERBASE + 0xf7ff)))
      *loc++ = (unsigned char) 0x00;
    else loc++;
  
  printf("\nECB Master DPRAM cleared (0x00 written to all accessable locations\n");
}






