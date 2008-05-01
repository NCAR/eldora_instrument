/*  Write one unsigned char to ECBMASTER CARD Non-volatile RAM.
    WRITES ONLY TO SRAM ARRAY: DOES NOT TRANSFER ARRAY TO EEPROM. */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

nvstorenum(offset,value)

unsigned short offset;
unsigned char value;
     
{
  unsigned char *nvram;
  nvram  = (unsigned char *)(MASTERBASE + MASTERNVRAM + offset);
  
  printf("nvstorenum(offset,value), will store one unsigned char (value)\n");
  printf("in NVRAM, at the address specified by \"offset\". Writes only to\n");
  printf("the SRAM array, and does not transfer the SRAM array to EEPROM.\n");

  *nvram = value;
  printf("%2x (hex) was written at offset %4x (hex)\n",value,offset);
}
