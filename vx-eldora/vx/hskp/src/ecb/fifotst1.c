/*
 *	$Id$
 *
 *	Module:		 fifotst1
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	fifotst1()                                                           */
/*              Repeately write to ECB MASTER IN FIFO, pause, read OUT FIFO  */
/*              to verify master has read the IN FIFO and transferred the    */
/*              contents back to the OUT FIFO.                               */
/*                                                                           */
/*              mstrfif1 should be running in the ECB master.                */
/*      Return values:                                                       */
/*              void, infinite loop function, no return values.              */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

#define WTIME 3000

void cuterotate(unsigned short val);

void fifotst1()

{
    unsigned char *vmectl,*vmestat,*infifo,*outfifo;
    unsigned char readback;
    unsigned short ivalue,jvalue;
    unsigned long totalemptys=0,totalerrs=0;
    
    vmectl  = (unsigned char *)(MASTERBASE + MASTERCTL);
    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo = (unsigned char *)(MASTERBASE + MASTEROUT);
    
    *vmectl = 0xF0;  /* clear IN and OUT FIFO's, keeping HC11 on */
    *vmectl = 0x10;  /* release FIFO clear signals, keeping HC11 on */
    if ((*vmestat & 0x01) != 0x00)
      printf("IN FIFO not empty after clear!!\n");
    
    while(1)
      {
	  printf("Writing 4090 bytes to IN FIFO...\n");
	  for(jvalue=0; jvalue<4090; jvalue++)
	    {
		cuterotate(jvalue);
		while ((*vmestat & 0x02) == 0x00)
		  printf("."); /* wait here if HALF-FULL */
		*infifo = (unsigned char) (jvalue & 0x00ff); /*write IN FIFO*/
		if ((*vmestat & 0x01) != 0x01)
		  {
		      printf("IN FIFO empty after a write!\n");
		      totalemptys++;
		  }
	    }
	  
	  *vmectl = 0x30;  /* turn on LED, keeping HC11 on */
	  for(ivalue=0; ivalue<WTIME; ivalue++);  /* wait awhile */
      
	  printf("Reading 4090 bytes from OUT FIFO...\n");
	  for(jvalue=0; jvalue<4090; jvalue++)
	    {
		cuterotate(jvalue);  /* make a cute little rotating thing */
		while ((*vmestat & 0x04) == 0x00)
		  printf("."); /* wait here if EMPTY */
		readback = *outfifo; /* read OUT FIFO */
		if (readback != (unsigned char) (jvalue & 0x00ff))
		  {
		      printf("unexpected value read back from OUT FIFO.\n");
		      printf("value expected was %d, ",(unsigned char) (jvalue & 0x00ff));
		      printf("value read back was %d\n",readback);
		      totalerrs++;
		  }
	    }
	  *vmectl = 0x10;  /* turn off LED, keeping HC11 on */
	  printf("ERROR TALLY: %ld total errors, %ld total not-empty-after-write's\n\n",totalerrs,totalemptys);
      } /* repeat forever */
}

    
void cuterotate(unsigned short val)

/*  print a cute little rotating thing;   val determines rotation to print */

{    
    switch(val%8) 
      {
	case 0:
	  printf("\\\r");
	  return;
	case 1:
	  printf("|\r");
	  return;
	case 2:
	  printf("/\r");
	  return;
	case 3:
	  printf("-\r");
	  return;
	case 4:
	  printf("\\\r");
	  return;
	case 5:
	  printf("|\r");
	  return;
	case 6:
	  printf("/\r");
	  return;
	case 7:
	  printf("-\r");
	  return;
      }  /* end of switch */
} /* end of function */
