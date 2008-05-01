/*
  
  Diagnostic to repeatedly transfer EEPROM to SRAM
  
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

loopes(count)
     
     unsigned int count;
     
{
  
  unsigned int i;
  
  while(1)
    {
      for (i=0; i<count; i++) printf("counting %d \r");
      printf ("Transferring EEPROM to SRAM\n");
      eetosram();
    }
}
