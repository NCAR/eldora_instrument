/*
  
  Diagnostic to repeatedly transfer SRAM to EEPROM
  
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

loopse(count)
     
     unsigned int count;
     
{
  
  unsigned int i;
  
  while(1)
    {
      for (i=0; i<count; i++) printf("counting %d \r");
      printf ("Transferring SRAM to EEPROM\n");
      sramtoee();
    }
}
