/*
 *	$Id$
 *
 *	Module:	pwr1.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:45:49  reif
 * Initial revision
 *
 * Revision 1.1  1992/08/14  21:43:30  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the two WAVETEK 8502A
 *              Peak Power Meters used for fore and aft peak power 
 *              measurements.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

/******************** MAIN PROGRAM ****************************/

void pwr_menu()
{
  int i,number_times;
  unsigned char response;
  unsigned char string_array[15];
  unsigned int choice;
/************* INITIALIZE POINTERS AND ISR VECTORS ************/

  init_ptrs();
  init_isr_vecs();

/******************* INITIALIZE GPIB AND DMA **********************/

  init_gpib();
  init_dma();

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

do
  { 
      getchar();
      puts("ENTER A COMMAND STRING");
      gets(string_array); /* GET COMMAND STRING FROM KEYBOARD */
      puts("SELECT POWER METER TO COMMAND");
      puts("1) FOR XMIT POWER METER");
      puts("2) FOR TESTP POWER METER");
      puts("3) FOR BOTH POWER METERS");
      choice=getchar();
      getchar();
      switch(choice)
	{
	  case '1':
	    send_cmnd_string(XMIT,string_array);
	    break;
	  case '2':
	    send_cmnd_string(TESTP,string_array);
	    break;
	  case '3':
	    send_cmnd_string(XMIT_AND_TESTP,string_array);
	    break;
	  default:
	    puts("NOT A CHOICE! TRY AGAIN");
	    break;
	}

      puts("DISPLAY DATA?");
      response=getchar(); /* CHECK TO SEE IF WE WANT TO COLLECT DATA */
      if (response== 'y')   /* IF SO, TURN BUS AROUND TO LISTEN */
	{

	    /****** TURN PPM INTO TALKER, MZ7500 INTO LISTENER ******/

	    listen();

	    /*********** START DMA CHANNEL 0 ***************/

	    *d0csr=0xff; /* Clear channel status register by writing all ones */
	    *d0ccr=0xC8; /* Set start, continue, and interrupt bits in */
	                 /* channel control register */

	    /*********** START DMA CHANNEL 1 **************/
	    
	    *d1csr=0xff; /* Clear channel status register by writing all ones */
	    *d1ccr=0xC8; /* Set start, continue, and interrupt bits in the */
	                 /* channel control register */
	    puts("INPUT NUMBER OF TIMES TO DISPLAY DATA");
	    scanf(" %d",&number_times);
	    for(i=0; i<number_times; i++)
	      {
		  if(xmit_isr_done==1)
		    {
			flt_pt();
			printf("FORE_XMIT_PWR=%-10.3E watts  AFT_XMIT_PWR=%-10.3E watts\n",fore_xmit_pwr,aft_xmit_pwr);
			xmit_isr_done=0;
		    }
		  else if(xmit_isr_done==-1)
		    {
			printf("NO DAMN GOOD\n");
			xmit_isr_done=0;
		    }
		  if(testp_isr_done==1)
		    {
			flt_pt(); 
			printf("FORE_TESTP_PWR = %-7.2f dBm  AFT_TESTP_PWR = %-7.2f dBm\n",fore_testp_pwr,aft_testp_pwr);
			testp_isr_done=0;
		    }
		  else if(testp_isr_done==-1)
		    {
			printf("NO DAMN GOOD\n");
			testp_isr_done=0;
		    }
	      }   
	}
      getchar(); /* CLEAR BUFFER */
      puts("Another Command?");
      response = getchar(); 
  }while (response == 'y'); /* CHECK TO SEE IF WE WANT TO CONTINUE OR ABORT */
}







