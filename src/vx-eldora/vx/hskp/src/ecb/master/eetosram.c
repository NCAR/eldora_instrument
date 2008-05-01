/*
 
Transfer ECB MASTER EEPROM array to SRAM.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

eetosram()

{
  unsigned char *nvxfr,dummy;
  
  nvxfr = (unsigned char *)(MASTERBASE + MASTERNVXFR);
  dummy = *nvxfr;
}
