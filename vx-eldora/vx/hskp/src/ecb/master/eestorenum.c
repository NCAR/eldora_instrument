/*  Write one unsigned char to ECBMASTER CARD Non-volatile RAM.
    WRITES TO SRAM ARRAY AND TRANSFERS ENTIRE ARRAY TO EEPROM. */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

eestorenum(offset,value)

unsigned short offset;
unsigned char value;
     
{
  unsigned char *nvram,*nvxfr;
  nvram  = (unsigned char *)(MASTERBASE + MASTERNVRAM + offset);
  nvxfr  = (unsigned char *)(MASTERBASE + MASTERNVXFR);
  
  printf("eestorenum(offset,value), will store one unsigned char (value)\n");
  printf("in NVRAM, at the address specified by \"offset\". Writes one value to the\n");
  printf("SRAM array, and then TRANSFERS ENTIRE ARRAY TO EEPROM!!!\n");
  *nvram = value;
  *nvxfr = value;
  printf("%2x (hex) was written at offset %4x (hex)\n",value,offset);
}
