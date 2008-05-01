/*
 
Write to ECB MASTER IN FIFO until half full, clear, repeat.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

fifotst(fullhalf)

unsigned short fullhalf;
     
{
  unsigned char *vmectl,*vmestat,*infifo,*clrinfifo;
  unsigned short ivalue,jvalue;
	
  
  vmectl    = (unsigned char *)(MASTERBASE + MASTERCTL);
  vmestat   = (unsigned char *)(MASTERBASE + MASTERSTAT);
  infifo    = (unsigned char *)(MASTERBASE + MASTERIN);
  jvalue = 1;

  while(1)
    {
      *vmectl = 0xC0;
      *vmectl = 0x00;  /* clear IN and OUT FIFO's */
      if ((*vmestat & 0x01) != 0x00) printf("IN FIFO not empty after clear!!\n");
      printf("Filling IN FIFO... ");
      if (jvalue == 8)
	jvalue = 1;
      else
	jvalue++;
      switch(jvalue)
	{
	case 1:
	  printf("\\\r");
	  break;
	case 2:
	  printf("|\r");
	  break;
	case 3:
	  printf("/\r");
	  break;
	case 4:
	  printf("-\r");
	  break;
	case 5:
	  printf("\\\r");
	  break;
	case 6:
	  printf("|\r");
	  break;
	case 7:
	  printf("/\r");
	  break;
	case 8:
	  printf("-\r");
	  break;
	}
      while ((*vmestat & 0x02) == 0x02) /* while NOT HALF-FULL */
	{
	  *infifo = 0x55;  /* write data into IN FIFO */
	  if ((*vmestat & 0x01) != 0x01) printf("IN FIFO empty after a write!\n");
	}
      *vmectl = 0x20;  /* turn on LED */
      for (ivalue = 0; ivalue < 1000; ivalue++);  /* wait awhile */
      *vmectl = 0x00;  /* turn off LED */
    } /* repeat forever */
}
