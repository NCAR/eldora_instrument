/*
 
Read and display ECB MASTER status register.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

status()
     
{
  unsigned char *vmestat;
  
  vmestat   = (unsigned char *)(MASTERBASE + 0xF300);
  
  printf("\n\n ECB MASTER Status buffer display: \n\n");

  while(1)
    {
      printf("status:  status buffer value: %2x\r",*vmestat);
    }
}
