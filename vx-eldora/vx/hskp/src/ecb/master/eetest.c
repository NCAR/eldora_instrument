/*  Repeatedly write/read test ECB MASTER CARD Non-Volatile RAM.  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

eetest()

{
  unsigned char *nvrambase,*nvloc,*nvxfer,dummy;
  unsigned short offset,num,nerr[512];
  unsigned int i;

  nvrambase = (unsigned char *)(MASTERBASE + MASTERNVRAM);
  nvxfer    = (unsigned char *)(MASTERBASE + MASTERNVXFR);

  printf("ELDORA UTILITY CARD NVRAM (EEPROM PORTION) MEMORY TEST:\n");
  printf("Entire SRAM array will be written, transferred to EEPROM,\n");
  printf("transferred back to SRAM, and verified.  All numbers between\n");
  printf("0 and 255 will be used sequentially as the data value.\n");

  for (i=0; i<512; i++)  nerr[i] = 0;

  for (num=0;num<256;num++)
    {
      printf("Testing with data = %d  ",num);
      for (offset=0;offset<512;offset++)  /* load SRAM array */
        {
          nvloc = nvrambase + offset;
          *nvloc = (unsigned char) num;
        }
      printf(".");
      *nvxfer = 0x00;  /* transfer SRAM to EEPROM */
      taskDelay(5);    /* wait */
      printf(".");
      dummy = *nvxfer; /* transfer EEPROM to SRAM */
      taskDelay(5);    /* wait */
      printf(".");
      for (offset=0;offset<512;offset++)  /* compare SRAM array */
        {
          nvloc = nvrambase + offset;
          if (*nvloc != (unsigned char) num)
            {
              nerr[offset]++;
              printf("MEMORY ERROR AT %x, data = %d\n",nvloc,num);
            }
        }
      printf(".\r");
    }
  printf("\nEEPROM memory test complete.  Error tally by location:\n");
  for (i=0; i<512; i++)
      if (nerr[i] != 0)
        printf("location %x had %d total errors.\n",i,nerr[i]);
}
