/*
 *	$Id$
 *
 *	Module:	start_ieee.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/09/20  17:37:01  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as talker and start DMA and auxilliary clock interrupt
 *              for real-time task (pwr.c). Also sets initial power meter
 *              cursor positions.
 *              
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"
#include "taskLib.h"

void start_ieee()
{
  unsigned char string_array[15], muxval, ecbaddr;
  unsigned char number_array[11], xmit_tp, test;
  int i,timeout;

  /* Initialize some global flags and counters */

  isr_count = 0;  /* Make these globals! */
  fore_aft = 0;
  freq_count = 1;
  tp_cnt = 0;

/************ INITIALIZE STRING AND NUMBER ARRAYS *************/

  for(i=0; i<11; i++)
    {
	number_array[i]=0;
    }
  for(i=0; i<15; i++)
    {
	string_array[i]=0;
    }

  init_gpib(1);
  init_gpib(2);
  xmit_csr_delay = (0.5 * (float)wave->chip_width[0] + (float)wave->chip_offset[0]) / 60.0; /* xmit pulsewidth in microseconds */
  testp_csr_delay = 1.5 * ((float)wave->gate_dist1[1] / 60.0); /* test pulse width is 3 times gate spacing in microseconds */

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

  strncpy(string_array,"FREQ\0",5); /* Select frequency */
  sprintf(number_array,"%7.2f",fraddes->freq1); /* Use freq 1 as nominal */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",ffrad->peak_power_offset); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",afrad->peak_power_offset); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"CDLA\0",5); /*Set cursor delay for fore xmit meter */  
  sprintf(number_array,"%9.2f",xmit_csr_delay);
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"CDLB\0",5); /*Set cursor delay for fore xmit meter */  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);
 
  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore tp meter */  
  sprintf(number_array,"%9.2f",testp_csr_delay);  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"CDLB\0",5); /* Set cursor delay for aft tp meter */  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  /************* PROGRAM Trigger MUX via ECB ****************/

  taskDelay(100);       /* delay 1 sec to see if it fixes ecb problem */
  ecbaddr = ECBATTEN;
  xmit_tp = 1; /* select xmit mux */
  muxval = 13;  /* select aft preknock as trigger */
  timeout = 0;
  do
    {
      if(timeout > 0)taskDelay(1);
      timeout++;
    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);

  taskDelay(100);       /* delay 1 sec to see if it fixes ecb problem */
  ecbaddr = ECBATTEN;
  xmit_tp = 0; /* select test pulse mux */
  muxval = 14; /* select aft testpulse as trigger */
  timeout = 0;
  do
    {
      if(timeout > 0)taskDelay(1);
      timeout++;
    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);


/*********** TURN PPM INTO TALKER, MZ7500 INTO LISTENER ************/

init_dma(1,15);
init_dma(2,24);

listen(1);
listen(2);

#ifdef CLOCK_ISR

/* Enable auxilliary clock interrupt */

sysAuxClkEnable();

#endif

/*********** START DMA CHANNEL 0 - GO! ***************/

  *d0csr=0xff; /* Clear channel status register by writing all ones */
  taskDelay(1);
  *d0ccr=0xC8; /* Set start, continue, and interrupt bits in */
               /* channel control register */

/*********** START DMA CHANNEL 1 - GO! **************/

  *d1csr=0xff; /* Clear channel status register by writing all ones */
  taskDelay(1);
  *d1ccr=0xC8; /* Set start, continue, and interrupt bits in the */
               /* channel control register */

taskRestart(xmit); /* re-start run-time power task */
taskRestart(testp); /* re-start run-time power task */

}



