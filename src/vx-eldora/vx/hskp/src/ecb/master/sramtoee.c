/*
 
Transfer ECB MASTER SRAM array to EEPROM.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

sramtoee()

{
  unsigned char *nvxfr,dummy;
  
  nvxfr = (unsigned char *)(MASTERBASE + MASTERNVXFR);
  dummy = 0x00;
  *nvxfr = dummy;
}
