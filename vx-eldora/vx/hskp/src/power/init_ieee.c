/*
 *	$Id$
 *
 *	Module:	init_ieee.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1997/08/26  15:54:42  craig
 * Initial revision
 *
 * Revision 1.1  1993/09/20  17:37:01  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the fore and
 *              aft WAVETEK 8502A Peak Power Meters used for the fore 
 *              and aft peak power measurements.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#define NUM_AVG  5
#include "hskpAll.h"

 void init_ieee()
{
  unsigned char string_array[15];
  unsigned char number_array[11];
  unsigned char muxval,ecbaddr,test,xmit_tp;
  int i,timeout;

xmit_pwr_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

/************ INITIALIZE STRING AND NUMBER ARRAYS *************/

  for(i=0; i<11; i++)
    {
	number_array[i]=0;
    }
  for(i=0; i<15; i++)
    {
	string_array[i]=0;
    }

/************* INITIALIZE POINTERS AND ISR VECTORS ************/

  init_ptrs();
  init_isr_vecs();
  Debug = 1;

/******************* INITIALIZE GPIB AND DMA **********************/

  init_gpib(1);
  init_gpib(2);
  init_dma(1,15); /* 15 bytes for xmit power meter */
  init_dma(2,24); /* 24 bytes for testpulse power meter */

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

  strncpy(string_array,"PKAB\0",5); /* Select peak dual channel mode */  
  send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */
 
  strncpy(string_array,"GRFA\0",5); /* Select graph mode */  
  send_cmnd_string(XMIT,string_array); /* on Xmit meter */

  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
  send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */

  strncpy(string_array,"FREQ\0",5); /* Select frequency */
  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
  send_cmnd_string(XMIT_AND_TESTP,string_array); /* for both power meters */
 
  strncpy(string_array,"WATT\0",5); /* Select linear power output mode for */ 
  send_cmnd_string(XMIT,string_array); /* the fore and aft xmit power meter */

  strncpy(string_array,"TRGE\0",5); /* Select external trigger for both */ 
  send_cmnd_string(XMIT_AND_TESTP,string_array); /* power meters */

  strncpy(string_array,"WDLA\0",5);  /* Select Window size */
  sprintf(number_array,"%7.2f",10.0); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);
  
  strncpy(string_array,"PRFA\0",5);  /* Select Power Reference */
  sprintf(number_array,"%7.2f",76.7); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"WDLB\0",5);  /* Select Window size */
  sprintf(number_array,"%7.2f",10.0); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);
  
  strncpy(string_array,"PRFB\0",5);  /* Select Power Reference */
  sprintf(number_array,"%7.2f",76.0); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"DBMW\0",5); /* Select dbm output mode for the fore */ 
  send_cmnd_string(TESTP,string_array); /* and aft testpulse power meter */

  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",0.0); /* fore testp power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",0.0); /* aft testp power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit and tp meter */  
  sprintf(number_array,"%9.2f",0.0);  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  strncpy(string_array,"RDLB\0",5); /* Set reference delay for aft xmit and tp meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit and tp meters*/  
  sprintf(number_array,"%9.2f",0.0);  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  strncpy(string_array,"SDLB\0",5); /* Set start delay for aft xmit and tp meters*/  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  Avg = NUM_AVG;
  sprintf(number_array,"%d",Avg);   /* was 500 */  
  strncpy(string_array,"AVPK\0",5); /* Set # points to avg for xmit meter*/  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  sprintf(number_array,"%d",100);  /* was 999 ; 100 worked */
  strncpy(string_array,"AVPK\0",5); /* Set # points to avg for tp meter*/  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

/* Spawn run-time power task */

xmit = taskSpawn("xmit",90,VX_FP_TASK,10000,(FUNCPTR)xmit_rt,0,0,0,0,0,0,0,0,0,0);
if(xmit == ERROR)
  printf("Failed to Spawn run-time XMIT task!\n");

testp = taskSpawn("testp",80,VX_FP_TASK,10000,(FUNCPTR)testp_rt,0,0,0,0,0,0,0,0,0,0);
if(testp == ERROR)
  printf("Failed to Spawn run-time TESTP task!\n");
taskSuspend(xmit);
taskSuspend(testp);
#ifdef CLOCK_ISR

/* set up auxilliary clock for 1Hz update rate */

  sysAuxClkRateSet(1);  

  if(sysAuxClkConnect((FUNCPTR)(xmit_pwr_isr),0) == ERROR)
  exit(2);
#endif
}
