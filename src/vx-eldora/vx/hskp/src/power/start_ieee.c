/*
 *	$Id$
 *
 *	Module:	start_ieee.c	 
 *	Original Author: Reif Heck 
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as talker and start DMA and auxilliary clock interrupt
 *              for real-time task (pwr.c). Also sets initial power meter
 *              cursor positions.
 *              
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#define NEW_START
#define NUM_AVG 5
#include "hskpAll.h"
#include "taskLib.h"

// Start the power meter handling.  Return 1 if we start successfully, 0 otherwise.
int start_ieee()
{
  unsigned char string_array[15], muxval, ecbaddr;
  unsigned char number_array[11], xmit_tp, test, status;
  int i,timeout, cmnd_stat;

  /* Initialize some global flags and counters */

  isr_count = 0;  /* Make these globals! */
  fore_aft = 1;
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

#ifdef NEW_START
/******************* INITIALIZE GPIB AND DMA **********************/

  init_gpib(1);
  init_gpib(2);
  init_dma(1,15); /* 15 bytes for xmit power meter */
  init_dma(2,24); /* 24 bytes for testpulse power meter */

#endif

  xmit_csr_delay = (0.5 * (float)wave->chip_width[0] + (float)wave->chip_offset[0]) / 60.0; /* xmit pulsewidth in microseconds */
  /*  testp_csr_delay = 1.5 * ((float)wave->gate_dist1[1] / 60.0); *//* test pulse width is 3 times gate spacing in microseconds */

  testp_csr_delay = 3.5 * ((float)wave->gate_dist1[1] / 60.0); /* test pulse width is 5 times gate spacing in microseconds */

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

#ifdef NEW_START

  strncpy(string_array,"PKAB\0",5); /* Select peak dual channel mode */  
  cmnd_stat = send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"GRFA\0",5); /* Select graph mode */  
  cmnd_stat = send_cmnd_string(XMIT,string_array); /* on Xmit meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array, "UNDD\0",5); /* Disable SRQ for underscale */  
  cmnd_stat = send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }
 
  strncpy(string_array,"UNDD\0",5); /* Disable SRQ for underscale */  
  cmnd_stat = send_cmnd_string(XMIT,string_array); /* on Xmit meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
  cmnd_stat = send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"FREQ\0",5); /* Select frequency */
  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(TESTP,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
  cmnd_stat = send_cmnd_string(XMIT_AND_TESTP,string_array); /* for both power meters */
  if(cmnd_stat == -1)
    {
      printf("Command: %s Not Received!\n",string_array);
      return 0;
    }
 
  strncpy(string_array,"WATT\0",5); /* Select linear power output mode for */ 
  cmnd_stat = send_cmnd_string(XMIT,string_array); /* the fore and aft xmit power meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"TRGE\0",5); /* Select external trigger for both */ 
  cmnd_stat = send_cmnd_string(XMIT_AND_TESTP,string_array); /* power meters */
  if(cmnd_stat == -1)
    {
      printf("Command: %s Not Received!\n",string_array);
      return 0;
    }

  strncpy(string_array,"WDLA\0",5);  /* Select Window size */
  sprintf(number_array,"%7.2f",6.0); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }
  
  strncpy(string_array,"PRFA\0",5);  /* Select Power Reference */
  sprintf(number_array,"%7.2f",76.7); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"WDLB\0",5);  /* Select Window size */
  sprintf(number_array,"%7.2f",6.0); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }
  
  strncpy(string_array,"PRFB\0",5);  /* Select Power Reference */
  sprintf(number_array,"%7.2f",76.0); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"DBMW\0",5); /* Select dbm output mode for the fore */ 
  cmnd_stat = send_cmnd_string(TESTP,string_array); /* and aft testpulse power meter */
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",0.0); /* fore testp power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(TESTP,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",0.0); /* aft testp power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(TESTP,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit and tp meter */  
  sprintf(number_array,"%9.2f",0.0);  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT_AND_TESTP,string_array);
  if(cmnd_stat == -1)
    {
      printf("Command: %s Not Received!\n",string_array);
      return 0;
    }

  strncpy(string_array,"RDLB\0",5); /* Set reference delay for aft xmit and tp meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT_AND_TESTP,string_array);
  if(cmnd_stat == -1)
    {
      printf("Command: %s Not Received!\n",string_array);
      return 0;
    }

  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit and tp meters*/  
  sprintf(number_array,"%9.2f",0.0);  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT_AND_TESTP,string_array);
  if(cmnd_stat == -1)
    {
      printf("Command: %s Not Received!\n",string_array);
      return 0;
    }

  strncpy(string_array,"SDLB\0",5); /* Set start delay for aft xmit and tp meters*/  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT_AND_TESTP,string_array);
  if(cmnd_stat == -1)
    {
      printf("Command: %s Not Received!\n",string_array);
      return 0;
    }

  Avg = NUM_AVG;
  sprintf(number_array,"%d",Avg);   /* was 500 */  
  strncpy(string_array,"AVPK\0",5); /* Set # points to avg for xmit meter*/  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  sprintf(number_array,"%d",100);  /* was 999 ; 100 worked */
  strncpy(string_array,"AVPK\0",5); /* Set # points to avg for tp meter*/  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(TESTP,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

#endif

  strncpy(string_array,"FREQ\0",5); /* Select frequency */
  sprintf(number_array,"%7.2f",fraddes->freq1); /* Use freq 1 as nominal */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",ffrad->peak_power_offset); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",afrad->peak_power_offset); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"CDLA\0",5); /*Set cursor delay for fore xmit meter */  
  sprintf(number_array,"%9.2f",xmit_csr_delay);
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"CDLB\0",5); /*Set cursor delay for fore xmit meter */  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(XMIT,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(1);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }
 
  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore tp meter */  
  sprintf(number_array,"%9.2f",testp_csr_delay);  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(TESTP,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

  strncpy(string_array,"CDLB\0",5); /* Set cursor delay for aft tp meter */  
  strncat(string_array,number_array,strlen(number_array));
  cmnd_stat = send_cmnd_string(TESTP,string_array);
  if(cmnd_stat == -1)
    {
      status = ReadStatusByte(2);
      printf("SRQ: %2X; Command: %s\n",status,string_array);
      return 0;
    }

//***********************************************************
// Tom 5/30/08 Trigger Mux now programmed by DRX
/*  
  
  //************* PROGRAM Trigger MUX via ECB ****************

  taskDelay(100);       // delay 1 sec to see if it fixes ecb problem 
  ecbaddr = ECBATTEN;
  xmit_tp = 1; // select xmit mux 
  //  muxval = 13;   select aft preknock as trigger 
  muxval = 5; // select fore preknock as trigger 
  timeout = 0;
  do
    {
      if(timeout > 0)taskDelay(1);
      timeout++;
    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);

  taskDelay(100);       // delay 1 sec to see if it fixes ecb problem 
  ecbaddr = ECBATTEN;
  xmit_tp = 0; // select test pulse mux 
  //  muxval = 14;  select aft testpulse as trigger 
  muxval = 6; // select fore testpulse as trigger 
  timeout = 0;
  do
    {
      if(timeout > 0)taskDelay(1);
      timeout++;
    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);

  //************* PROGRAM Trigger Divider via ECB ****************

  taskDelay(100);       // delay 1 sec to see if it fixes ecb problem 
  ecbaddr = ECBATTEN;
  xmit_tp = 1; // select xmit mux 
  muxval = 2; // set N = 2; this is Minimum 
  timeout = 0;
  do
    {
      if(timeout > 0)taskDelay(1);
      timeout++;
    }while((test = ecbSetDivN(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);

  taskDelay(100);       // delay 1 sec to see if it fixes ecb problem 
  ecbaddr = ECBATTEN;
  xmit_tp = 0; // select test pulse mux 
  muxval = 2; // set N = 2; this is Minimum 
  timeout = 0;
  do
    {
      if(timeout > 0)taskDelay(1);
      timeout++;
    }while((test = ecbSetDivN(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);

*/
//***********************************************************************

/*********** TURN PPM INTO TALKER, MZ7500 INTO LISTENER ************/

  /*
    init_dma(1,15);
    init_dma(2,24);
  */
listener(1);
listener(2);

#ifdef CLOCK_ISR

/* Enable auxilliary clock interrupt */

sysAuxClkEnable();

#endif

/* Finally, enable the interrupt */
sysIntEnable(IEEE_IRQ);

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
taskRestart(pgm);  /* re-start run-time power task */
taskRestart(xmit); /* re-start run-time power task */
taskRestart(testp); /* re-start run-time power task */

return 1; /* success! */
}



