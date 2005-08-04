/*
 *	$Id$
 *
 *	Module:	pwrmenu.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.6  2000/05/15  18:48:32  eric
 * Fixed ability to send power meters commands from the keyboard.
 *
 * Revision 1.5  1999/09/27  16:47:35  eric
 * switched power meter triggers from aft to fore
 *
 * Revision 1.4  1999/07/13  16:51:27  thor
 * *** empty log message ***
 *
 * Revision 1.3  1997/11/12 19:45:49  eric
 * modified to support Graph Mode for xmit pulse.
 * Also added semaphores and restructured code.
 *
 * Revision 1.2  1993/09/20  17:35:35  reif
 * *** empty log message ***
 *
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
#define Init
#define FULL_UP
#include "vxWorks.h"
#include "stdioLib.h"
#include "hskpAll.h"
#include "semLib.h"

static int firstime;

/******************** MAIN PROGRAM ****************************/

void pwr_menu()
{
  int i,number_times,timeout,freq_cnt,sem_status;
  unsigned char response,ecbaddr,xmit_tp,muxval,test;
  unsigned char string_array[15],number_array[11];
  unsigned int choice, choice_1;

taskSuspend(testp); /* suspend run-time power task */
taskSuspend(pgm); /* suspend run-time power task */
taskSuspend(xmit); /* suspend run-time power task */

/************ INITIALIZE STRING AND NUMBER ARRAYS *************/

  for(i=0; i<11; i++)
    {
        number_array[i]=0;
    }
  for(i=0; i<15; i++)
    {
        string_array[i]=0;
    }
firstime = 1;
#ifdef Init  /* Undefine once operational code is functional */

/************* INITIALIZE POINTERS AND ISR VECTORS ************/

  init_ptrs();
  init_isr_vecs();
  sysIntEnable(3); /* Enable IRQ3 */

#endif

/******************* INITIALIZE GPIB AND DMA **********************/

  init_gpib(1);
  init_gpib(2);
  init_dma(1,28); /* 28 bytes for xmit power meter */
  init_dma(2,24); /* 24 bytes for testpulse power meter */

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

freq_cnt = 1; /* initialize frequency counter */
do
  {
    printf(" \n");
    printf(" \n");
    printf("POWER MENU\n");
    printf(" \n");
    printf("1) CONFIGURE XMIT POWER METER - FORE AND AFT IN PEAK MODE\n");
    printf("2) CONFIGURE TESTP POWER METER - FORE AND AFT IN PEAK MODE\n");
    printf("3) CONFIGURE XMIT POWER METER - FORE IN GRAPH MODE\n");
    printf("4) CONFIGURE XMIT POWER METER - AFT IN GRAPH MODE\n");
    printf("5) SEND POWER METER A COMMAND\n");
    scanf(" %d",&choice);
    switch(choice)
      {	
      case 1:
	/*	  talk(1); */
	init_gpib(1);

	  strncpy(string_array,"PKAB\0",5); /* Select peak dual channel mode */  
	  send_cmnd_string(XMIT,string_array); /* on  Xmit power meter */

	  strncpy(string_array,"UNDD\0",5); /* Disable Service Requests for Under Range conditions */  
	  send_cmnd_string(XMIT,string_array); /* on  Xmit power meter */
	  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */

	  strncpy(string_array,"FREQ\0",5); /* Select frequency */
	  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  send_cmnd_string(XMIT,string_array); /* for Xmit meter */
	  strncpy(string_array,"TRGE\0",5); /* Select external trigger for  */ 
	  send_cmnd_string(XMIT,string_array); /* Xmit power meter */

	  strncpy(string_array,"WATT\0",5); /* Select watt output mode for the fore */ 
	  send_cmnd_string(XMIT,string_array); /* and aft xmit power meter */
	  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",ffrad->peak_power_offset); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",afrad->peak_power_offset); /* aft xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  printf("FORE XMIT OFFSET = %7.2f; AFT XMIT OFFSET = %7.2f\n",ffrad->peak_power_offset,afrad->peak_power_offset);
	  xmit_csr_delay = (0.5 * ((float)wave->chip_width[0] + (float)wave->chip_offset[0])/ 60.0); /* xmit pulsewidth in microseconds */

	  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit power meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"RDLB\0",5); /* Set reference delay for aft xmitpower meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit power meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"SDLB\0",5); /* Set start delay for aft xmit powet meter */  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore xmit power meter */  
	  sprintf(number_array,"%9.2f",xmit_csr_delay);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"CDLB\0",5); /* Set cursor delay for aft xmit power meter */  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  ecbaddr = ECBATTEN;
	  xmit_tp = 1; /* select xmit mux */
	  muxval = 13;  /* select aft preknock as trigger */
	  muxval = 5;  /* select fore preknock as trigger */
	  timeout = 0;
	  do
	    {
	      if(timeout > 0)taskDelay(1);
	      timeout++;
	    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);
	  break;
	case 2:
	  init_gpib(2);
	  /* talk(2); */
	  strncpy(string_array,"PKAB\0",5); /* Select peak dual channel mode */  
	  send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */

	  strncpy(string_array,"UNDD\0",5); /* Disable Service Requests for Under Range conditions */  
	  send_cmnd_string(XMIT,string_array); /* on  Xmit power meter */
	  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
	  send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */
	  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
	  send_cmnd_string(TESTP,string_array); /* for Test Pulse power meters */
	  strncpy(string_array,"TRGE\0",5); /* Select external trigger for  */ 
	  send_cmnd_string(XMIT_AND_TESTP,string_array); /* Test Pulse power meters */
	  strncpy(string_array,"FREQ\0",5); /* Select frequency */
	  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"DBMW\0",5); /* Select dbm output mode for the fore */ 
	  send_cmnd_string(TESTP,string_array); /* and aft testpulse power meter */
	  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",0.0); /* Fore Test Pulse power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",0.0); /* aft Test Pulse power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  printf("FORE TESTP OFFSET = %7.2f; AFT TESTP OFFSET = %7.2f\n",ffrad->test_pulse_offset,afrad->test_pulse_offset);

	  testp_csr_delay = 1.5 * ((float)wave->gate_dist1[1] / 60.0); /* test pulse width is 3 times gate spacing in microseconds */

	  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit and tp meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"RDLB\0",5); /* Set reference delay for aft xmit and tp meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit and tp meters*/  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"SDLB\0",5); /* Set start delay for aft xmit and tp meters*/  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore tp meter */  
	  sprintf(number_array,"%9.2f",testp_csr_delay);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  strncpy(string_array,"CDLB\0",5); /* Set cursor delay for aft tp meter */  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(TESTP,string_array);

	  ecbaddr = ECBATTEN;
	  xmit_tp = 0; /* select test pulse mux */
	  muxval = 14;  /* select aft testpulse as trigger */
	  muxval = 6;  /* select fore testpulse as trigger */
	  timeout = 0;
	  do
	    {
	      if(timeout > 0)taskDelay(1);
	      timeout++;
	    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);
	  break;
	case 3:
	  init_gpib(1);
	  /* talk (1) */

	  strncpy(string_array,"GRFA\0",5); /* Select graph mode */  
	  send_cmnd_string(XMIT,string_array); /* on  Xmit power meter */
	  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */

	  send_cmnd_string(XMIT,string_array); /* for Xmit meter */
	  strncpy(string_array,"TRGE\0",5); /* Select external trigger for  */ 
	  send_cmnd_string(XMIT,string_array); /* Xmit power meter */

	  strncpy(string_array,"WDLA\0",5);  /* Select Window size */
	  sprintf(number_array,"%7.2f",10.00); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"PRFA\0",5);  /* Select Power Reference */
	  sprintf(number_array,"%7.2f",50.00); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"WATT\0",5); /* Select watt output mode for the fore */ 
	  send_cmnd_string(XMIT,string_array); /* and aft xmit power meter */
	  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",ffrad->peak_power_offset); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  printf("FORE XMIT OFFSET = %7.2f\n",ffrad->peak_power_offset);

	  strncpy(string_array,"FREQ\0",5); /* Select frequency */
	  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  xmit_csr_delay = (0.5 * (float)wave->chip_width[0] + (float)wave->chip_offset[0]) / 60.0; /* xmit pulsewidth in microseconds */

	  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit power meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit power meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore xmit power meter */  
	  sprintf(number_array,"%9.2f",xmit_csr_delay);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

#ifdef FULL_UP
	  strncpy(string_array,"UPDT\0",5); /* Select update now mode */ 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  strncpy(string_array,"UPDN\0",5); 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  init_dma(1,15); /* init dma for single channel operation */
#endif
#ifndef FULL_UP
	  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  init_dma(1,14); /* init dma for single channel operation */
#endif
	  ecbaddr = ECBATTEN;
	  xmit_tp = 1; /* select xmit mux */
	  muxval = 5;  /* select fore preknock as trigger */
	  muxval = 13; /* select aft preknock as trigger */
	  timeout = 0;
	  do
	    {
	      if(timeout > 0)taskDelay(1);
	      timeout++;
	    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);
	  break;

      case 4:
	/*	  talk(1); */
	init_gpib(1);
	  strncpy(string_array,"GRFB\0",5); /* Select graph mode */  
	  send_cmnd_string(XMIT,string_array); /* on  Xmit power meter */
	  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
	  send_cmnd_string(XMIT,string_array); /* for Xmit meter */
	  strncpy(string_array,"TRGE\0",5); /* Select external trigger for  */ 
	  send_cmnd_string(XMIT,string_array); /* Xmit power meter */

	  strncpy(string_array,"WDLB\0",5);  /* Select Window size */
	  sprintf(number_array,"%7.2f",10.00); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"PRFB\0",5);  /* Select Power Reference */
	  sprintf(number_array,"%7.2f",50.00); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"WATT\0",5); /* Select watt output mode for the fore */ 
	  send_cmnd_string(XMIT,string_array); /* and aft xmit power meter */

	  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",afrad->peak_power_offset); /* aft xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  printf("AFT XMIT OFFSET = %7.2f\n",afrad->peak_power_offset);

	  strncpy(string_array,"FREQ\0",5); /* Select frequency */
	  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  xmit_csr_delay = (0.5 * (float)wave->chip_width[0] + (float)wave->chip_offset[0]) / 60.0; /* xmit pulsewidth in microseconds */

	  strncpy(string_array,"RDLB\0",5); /* Set reference delay for aft xmitpower meter */
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

	  strncpy(string_array,"SDLB\0",5); /* Set start delay for aft xmit powet meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);
	  strncpy(string_array,"CDLB\0",5); /* Set cursor delay for aft xmit power meter */  
	  sprintf(number_array,"%9.2f",xmit_csr_delay); 
	  strncat(string_array,number_array,strlen(number_array));
	  send_cmnd_string(XMIT,string_array);

#ifdef FULL_UP
	  strncpy(string_array,"UPDT\0",5); /* Select update now mode */ 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  strncpy(string_array,"UPDN\0",5); 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  init_dma(1,15); /* init dma for single channel operation */
#endif
#ifndef FULL_UP
	  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
	  send_cmnd_string(XMIT,string_array); /* on Xmit power meter */
	  init_dma(1,14); /* init dma for single channel operation */
#endif
	  ecbaddr = ECBATTEN;
	  xmit_tp = 1; /* select xmit mux */
	  muxval = 5;  /* select fore preknock as trigger */
	  muxval = 13; /* select aft preknock as trigger */
	  timeout = 0;
	  do
	    {
	      if(timeout > 0)taskDelay(1);
	      timeout++;
	    }while((test = ecbSetMux(ecbaddr,xmit_tp,muxval) != 0) && timeout < 30);
	  break;
      case 5:
	printf("ENTER A COMMAND STRING\n");
	scanf(" %s",string_array); /* GET COMMAND STRING FROM KEYBOARD */
	printf("SELECT POWER METER TO COMMAND\n");
	printf("1) FOR XMIT POWER METER\n");
	printf("2) FOR TESTP POWER METER\n");
	printf("3) FOR BOTH POWER METERS\n");
	scanf(" %d",&choice_1);
	switch(choice_1)
	  {
	  case 1:
	    /*	    talk(1); */
	    init_gpib(1);
	    send_cmnd_string(XMIT,string_array);
	    break;
	  case 2:
	    /*	    talk(2); */
	    init_gpib(2);
	    send_cmnd_string(TESTP,string_array);
	    break;
	  case 3:
	    /*	    talk(1);
	    talk(2); */
	    init_gpib(1);
	    init_gpib(2);
	    send_cmnd_string(XMIT_AND_TESTP,string_array);
	    break;
	  default:
	    printf("NOT A CHOICE! TRY AGAIN\n");
	    break;
	  }
	break;
      default:
	printf("NOT A CHOICE! TRY AGAIN\n");
	break;
      }
      printf("DISPLAY DATA?\n");
      scanf(" %c",&response); /* CHECK TO SEE IF WE WANT TO COLLECT DATA */
      if (response== 'y')   /* IF SO, TURN BUS AROUND TO LISTEN */
	{
	  printf("INPUT NUMBER OF TIMES TO DISPLAY DATA\n");
	  scanf(" %d",&number_times);

	    /****** TURN PPM INTO TALKER, MZ7500 INTO LISTENER ******/

	  if(choice == 2)
	    {
	      listener(2);

	    /*********** START DMA CHANNEL 1 **************/
	    
	      *d1csr=0xff; /* Clear channel status register by writing all ones */
	      taskDelay(1);
	      *d1ccr=0xC8; /* Set start, continue, and interrupt bits in the */
	                 /* channel control register */
	    }
	  else
	    {
	      listener(1);

	    /*********** START DMA CHANNEL 0 ***************/

	    *d0csr=0xff; /* Clear channel status register by writing all ones */
	    taskDelay(1);
	    *d0ccr=0xC8; /* Set start, continue, and interrupt bits in */
	                 /* channel control register */

	    }
	    for(i=0; i<number_times; i++)
	      {
		if(choice == 1 || choice == 3 || choice == 4)
		  {
		    sem_status = semTake(xmit_data_sem,1000); /* extend wait for graph mode which averages and draws */
		    if(sem_status == OK)
		      {
			if(choice == 1)
			  flt_pt(0); /* Update Continously Mode */
#ifndef FULL_UP
			if(choice == 3 || choice == 4)
			  flt_pt(0); /* Update Continuously Mode */
#endif

#ifdef FULL_UP
			if(choice == 3 || choice == 4)
			  {
			    if(freq_cnt > fraddes->num_freq_trans)
			      freq_cnt = 1;
			    flt_pt(1);
			    *d0ccr = 0x10; /* Abort channel 1 DMA operation */
			    taskDelay(1);
			    *d0csr = 0xff; /* Clear DMA status register */

			    /* try clearing GPIB Interface */

			    *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
			    *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
			    taskDelay(1);  /* SET DELAY */
			    *g1acr = 0x0f; /* CLEAR sic */	    

			    /* handle any pending interrupts */

			    sem_status = semTake(xmit_data_sem,NO_WAIT);
			    sem_status = semTake(xmit_err_sem,NO_WAIT);
			    init_gpib(1); /* try this here to re-establish comms */
			    /*	 talk(1); */
			    
			    xmit_csr_delay = ((0.5 + freq_cnt-1) * (float)wave->chip_width[0] + (float)wave->chip_offset[0]) / 60.0; /* xmit pulsewidth in microseconds */
			  
			    if(choice == 3)
			      {
				strncpy(string_array,"CDLA\0",5); /*Set cursor delay for fore xmit meter */  
				sprintf(number_array,"%9.2f",xmit_csr_delay);
				strncat(string_array,number_array,strlen(number_array));
				send_cmnd_string(XMIT,string_array);
			      }
			    if(choice == 4)
			      {
				strncpy(string_array,"CDLB\0",5); /*Set cursor delay for fore xmit meter */  
				sprintf(number_array,"%9.2f",xmit_csr_delay);
				strncat(string_array,number_array,strlen(number_array));
				send_cmnd_string(XMIT,string_array);
			      }
			    taskDelay(100);
			    strncpy(string_array,"UPDT\0",5); /* Select update trigger reset mode */ 
			    send_cmnd_string(XMIT,string_array); /* on Xmit meter */

			    strncpy(string_array,"UPDN\0",5); /* Select update now mode */ 
			    send_cmnd_string(XMIT,string_array); /* on Xmit meter */
			  }

#endif
			if(choice == 1)
			  printf("FORE_XMIT_PWR=%-10.3E watts  AFT_XMIT_PWR=%-10.3E watts\n",fore_xmit_pwr,aft_xmit_pwr);
			
			if(choice == 3)
			  printf("FORE_XMIT_PWR=%-10.3E watts\n",fore_xmit_pwr);
			if(choice == 4)
			  printf("AFT_XMIT_PWR=%-10.3E watts\n",fore_xmit_pwr);
#ifdef FULL_UP	
			if(choice == 3 || choice == 4)
			  {
			    freq_cnt++;
			    init_dma(1,15);			    
			    taskDelay(1);
			    listener(1);
			    *d0csr = 0xff;
			    taskDelay(1);
			    *d0ccr = 0xC8;  /* start DMA */
			  }
#endif
			xmit_isr_done=0;
			taskDelay(1);
			if(choice == 1)
			  *d0ccr = 0x48;
#ifndef FULL_UP
			*d0ccr = 0x48;
#endif
		      }
		    else
		      printf("NO XMIT DATA INTERRUPT\n");
		    sem_status = semTake(xmit_err_sem,NO_WAIT);
		    if(sem_status == OK)
		      {
			printf("NO DAMN GOOD: ERROR = %d\n",dma_err1);
			xmit_isr_done=0;
#ifdef FULL_UP			
			init_dma(1,15);
#endif
#ifndef FULL_UP			
			init_dma(1,14);
#endif
			*d0csr = 0xff;
			taskDelay(1);
			*d0ccr = 0xC8;  /* start DMA */
		      }
		  }
		else
		  {
		    sem_status = semTake(testp_data_sem,100);
		    if(sem_status == OK)
		      {
			flt_pt(0); /* Update Continuously Mode */ 
			printf("FORE_TESTP_PWR = %-7.2f dBm  AFT_TESTP_PWR = %-7.2f dBm\n",fore_testp_pwr,aft_testp_pwr);
			testp_isr_done=0;
			taskDelay(1);
			*d1ccr = 0x48;
		      }
		    else
		      printf("NO TESTP DATA INTERRUPT\n");
		    sem_status = semTake(testp_err_sem,NO_WAIT);
		    if(sem_status == OK)
		      {
			printf("NO DAMN GOOD: ERROR = %d\n",dma_err2);
			testp_isr_done=0;
			init_dma(2,24);
			*d1csr = 0xff;  /* clear status register */
			taskDelay(1);
			*d1ccr = 0xC8;  /* start DMA */
		      }
		  }
	      }

	    *d0ccr = 0x10; /* Abort channel 1 DMA operation */
	    *d1ccr = 0x10; /* Abort channel 2 DMA operation */
	    *d0csr = 0xff; /* Clear DMA status register */
	    *d1csr = 0xff; /* Clear DMA status register */

	    /* try clearing GPIB Interface */

	    *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
	    *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
	    taskDelay(1);  /* SET DELAY */
	    *g1acr = 0x0f; /* CLEAR sic */
	    
	    *g2acr = 0x00; /* CLEAR SOFTWARE RESET */
	    *g2acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
	    taskDelay(1);  /* SET DELAY */
	    *g2acr = 0x0f; /* CLEAR sic */

	    /* handle any pending interrupts */

	    sem_status = semTake(xmit_data_sem,NO_WAIT);
	    sem_status = semTake(xmit_err_sem,NO_WAIT);
	    sem_status = semTake(testp_data_sem,NO_WAIT);
	    sem_status = semTake(testp_err_sem,NO_WAIT);
	}
      printf("Another Command?\n");
      scanf(" %c",&response); 
  }while (response == 'y'); /* CHECK TO SEE IF WE WANT TO CONTINUE OR ABORT */
#ifdef Init
  sysIntDisable(3); /* disable IRQ3 */
#endif
  taskRestart(pgm); /* re-start run-time power task */
  taskRestart(xmit); /* re-start run-time power task */
  taskRestart(testp); /* re-start run-time power task */
}







