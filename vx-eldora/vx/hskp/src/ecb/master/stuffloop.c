/*  
  Poke an infinite loop program into ECB MASTER DPRAM
  Places a jump extended (JMP) opcode at the requested address, followed
  by the address, thus creating an infinite loop.
  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

void stuffloop(addr)
     
     unsigned short addr;
{
  unsigned char addrh,addrl,*loc;
  
  loc = (unsigned char *) (MASTERBASE + addr);
  addrl = addr & 0xff;
  addrh = addr>>8;
  printf("addr,addrh,addrl ===> %4x,%2x,%2x\n",addr,addrh,addrl);

  *loc++ = 0x7e;  /* stuff JMP opcode at addr, increment pointer */
  *loc++ = addrh; /* stuff MSB of addr, increment pointer */
  *loc = addrl;   /* stuff LSB of addr */

}






