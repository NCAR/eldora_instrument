/*
 *	$Id$
 *
 *	Module:	lab_test.c	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  2000/05/15  18:43:24  eric
 * Initial revision
 *
 * Initial revision
 *
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from a single WAVETEK 8502A
 *              Peak Power Meter used for fore and aft peak power 
 *              measurements.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#define Init
#define NO_FULL_UP
#include "vxWorks.h"
#include "stdio.h"
#include "stdioLib.h"
#include "hskpAll.h"
#include "semLib.h"

static int firstime;

/******************** MAIN PROGRAM ****************************/

void lab_test()
{
  int i,number_times,timeout,freq_cnt,sem_status,cmnd_stat, meter;
  unsigned char response,ecbaddr,xmit_tp,muxval,test;
  unsigned char string_array[15],number_array[11],*data;
  unsigned int choice, choice_1;
  unsigned char status;
  char ppm;

/************ INITIALIZE STRING AND NUMBER ARRAYS *************/

  for(i=0; i<11; i++)
    {
        number_array[i]=0;
    }
  for(i=0; i<15; i++)
    {
        string_array[i]=0;
	data[i] = 0;
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
  /*  init_dma(1,28); *//* 28 bytes for xmit power meter */
  init_dma(2,13); /* 13 bytes for testpulse power meter */
  init_dma(1,13); /* 13 bytes for transmit power meter */

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

freq_cnt = 1; /* initialize frequency counter */
do
  {
    printf(" \n");
    printf(" \n");
    printf("POWER MENU\n");
    printf(" \n");
    printf("1) CONFIGURE POWER METER IN PEAK MODE\n");
    printf("2) CONFIGURE POWER METER IN GRAPH MODE\n");
    printf("3) TEST TALK/LISTEN FUNCTION\n");
    printf("4) SEND POWER METER A COMMAND\n");
    scanf(" %d",&choice);
    switch(choice)
      {	
	case 1:

	  printf("Enter 1 for XMIT or 2 for TESTP\n");
	  scanf(" %d",&meter);
	  if(meter != 1 && meter != 2)
	    {
	      printf("Not a Choice! %d\n",meter);
	      break;
	    }
	  strncpy(string_array,"PKPA\0",5); /* Select cw channel A mode */  
	  cmnd_stat = send_cmnd_string(meter,string_array); /* on Test Pulse meter */

	  strncpy(string_array,"UNDD\0",5); /* Disable Service Requests for Under Range conditions */  
	  cmnd_stat = send_cmnd_string(meter,string_array); /* on Test Pulse meter */

	  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
	  cmnd_stat = send_cmnd_string(meter,string_array); /* for Test Pulse power meters */
	  strncpy(string_array,"TRGA\0",5); /* Select internal trigger for  */ 
	  sprintf(number_array,"%7.2f",-15.00); /* Use -15 dBm as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array); /* Test Pulse power meters */
	  strncpy(string_array,"FREQ\0",5); /* Select frequency */
	  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"DBMW\0",5); /* Select dbm output mode for the fore */ 
	  cmnd_stat = send_cmnd_string(meter,string_array); /* and aft testpulse power meter */

	  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",0.0); /* Fore Test Pulse power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit and tp meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit and tp meters*/  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore tp meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);
	  break;
	case 2:

	  printf("Enter 1 for XMIT or 2 for TESTP\n");
	  scanf(" %d",&meter);
	  if(meter != 1 && meter != 2)
	    {
	      printf("Not a Choice!\n");
	      break;
	    }
	  strncpy(string_array,"GRFA\0",5); /* Select graph mode */  
	  cmnd_stat = send_cmnd_string(meter,string_array); /* on  TESTP power meter */
	  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
	  cmnd_stat = send_cmnd_string(meter,string_array); /* for TESTP meter */

	  strncpy(string_array,"TRGA\0",5); /* Select external trigger for  */ 
	  sprintf(number_array,"%7.2f",-15.00); /* Use -15 dBm as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array); /* TESTP power meter */

	  strncpy(string_array,"WDLA\0",5);  /* Select Window size */
	  sprintf(number_array,"%7.2f",1.00); /* fore TESTP power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"DBMW\0",5); /* Select dBm output mode for the fore */ 
	  cmnd_stat = send_cmnd_string(meter,string_array); /* and aft TESTP power meter */

	  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
	  sprintf(number_array,"%7.2f",0.0); /* fore TESTP power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"PRFA\0",5);  /* Select Power Reference */
	  sprintf(number_array,"%7.2f",-10.0); /* fore xmit power meter */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"FREQ\0",5); /* Select frequency */
	  sprintf(number_array,"%7.2f",9.5); /* Use 9.5 GHz as nominal */
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  xmit_csr_delay = 0.0;

	  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore TESTP power meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore TESTP power meter */  
	  sprintf(number_array,"%9.2f",0.0);  
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);

	  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore TESTP power meter */  
	  sprintf(number_array,"%9.2f",xmit_csr_delay);  
	  strncat(string_array,number_array,strlen(number_array));
	  cmnd_stat = send_cmnd_string(meter,string_array);
	  break;
      case 3:
	printf("Enter 1 for XMIT or 2 for TESTP\n");
	scanf(" %d",&meter);
	if(meter != 1 && meter != 2)
	  {
	    printf("Not a Choice!\n");
	    break;
	  }

	    printf("Enter Number of times to test\n");
	    scanf(" %d",&choice_1);
	    xmit_csr_delay = 0.0;
	    for(i=0; i<choice_1; i++)
	      {
		if(xmit_csr_delay > 0.4)
		  xmit_csr_delay = 0.0;
		strncpy(string_array,"UPDT\0",5); /* Select update t
rigger reset mode */ 
		cmnd_stat = send_cmnd_string(meter,string_array); /* on Testp meter */
	       
		strncpy(string_array,"UPDN\0",5); /* Select update n
ow mode */ 
		cmnd_stat = send_cmnd_string(meter,string_array); /* on Testp mete
r */
		if(meter == 1)
		  while(*g1ism1 != 0x2) 	/* Wait for SRQ from PPM */
		    taskDelay(1);
		if(meter == 2)
		  while(*g2ism1 != 0x2) 	/* Wait for SRQ from PPM */
		    taskDelay(1);
		data = ReceiveData(meter,13);
		printf("%s\n",data);
		taskDelay(200);
		talk(meter);
		strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore TESTP power meter */  
		sprintf(number_array,"%9.2f",xmit_csr_delay);  
		strncat(string_array,number_array,strlen(number_array));
		cmnd_stat = send_cmnd_string(meter,string_array);
		xmit_csr_delay += 0.1;
		
	      }
	break;
      case 4:
	printf("Enter 1 for XMIT or 2 for TESTP\n");
	scanf(" %d",&meter);
	if(meter != 1 && meter != 2)
	  {
	    printf("Not a Choice!\n");
	    break;
	  }
	printf("ENTER A COMMAND STRING:\n ");
	scanf(" %s",string_array); /* GET COMMAND STRING FROM KEYBOARD */
	cmnd_stat = send_cmnd_string(meter,string_array);
	if(cmnd_stat == -1)
	  {
	    status = ReadStatusByte(meter);
	    printf("SRQ: %2X; Command: %s\n",status,string_array);
	  }
	break;
      }
    if(choice == 1 || choice == 2)
      {
	printf("DISPLAY DATA?\n");
	scanf(" %c",&response); /* CHECK TO SEE IF WE WANT TO COLLECT DATA */
	if (response== 'y')   /* IF SO, TURN BUS AROUND TO LISTEN */
	  {
	    printf("INPUT NUMBER OF TIMES TO DISPLAY DATA\n");
	    scanf(" %d",&number_times);
	    for(i=0; i<number_times; i++)
	      {

		if(choice == 1)
		  {
		    talk(meter);
		    strncpy(string_array,"UPDT\0",5); /* Select update t
							 rigger reset mode */ 
		    cmnd_stat = send_cmnd_string(meter,string_array); /* on Testp meter */
		    
		    strncpy(string_array,"UPDN\0",5); /* Select update now mode */ 
		    cmnd_stat = send_cmnd_string(meter,string_array); /* on TESTP power meter */
		    taskDelay(1);
		    init_dma(meter,13); /* init dma for single channel operation */    
		  }
		if(choice == 2)
		  {
		    talk(meter);
		    if(xmit_csr_delay > 0.4)
		      xmit_csr_delay = 0.0;
		    strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore TESTP power meter */  
		    sprintf(number_array,"%9.2f",xmit_csr_delay);  
		    strncat(string_array,number_array,strlen(number_array));
		    cmnd_stat = send_cmnd_string(meter,string_array);
		    xmit_csr_delay += 0.1;

		    strncpy(string_array,"UPDT\0",5); /* Select update t
							 rigger reset mode */ 
		    cmnd_stat = send_cmnd_string(meter,string_array); /* on Testp meter */
		    
		    strncpy(string_array,"UPDN\0",5); /* Select update now mode */ 
		    cmnd_stat = send_cmnd_string(meter,string_array); /* on TESTP power meter */

		    taskDelay(1);
		    init_dma(meter,13); /* init dma for single channel operation */
		  }
	    
	    /****** TURN PPM INTO TALKER, MZ7500 INTO LISTENER ******/
	    
		taskDelay(1);
		listener(meter);
	    
	    /*********** START DMA CHANNEL  **************/
	    
		if(meter == 1)
		  {
		    *d0csr=0xff; /* Clear channel status register by writing all ones */
		    taskDelay(1);
		    *d0ccr=0xC8; /* Set start, continue, and interrupt bits in the */
		  }

		if(meter == 2)
		  {
		    *d1csr=0xff; /* Clear channel status register by writing all ones */
		    taskDelay(1);
		    *d1ccr=0xC8; /* Set start, continue, and interrupt bits in the */
		  }

		/* channel control register */
	    
		if(choice == 2)
		  {
		    if(meter == 1)
		      {
			sem_status = semTake(xmit_data_sem,1000); /* extend wait for graph mode which averages and draws */
			if(sem_status == OK)
			  {
			    printf("count: %d %s\n",i,(xmit_array + STANDARD_BASE));
			    xmit_isr_done=0;
			    taskDelay(1);
			    *d0ccr = 0x48;
			  }
			else
			  printf("NO XMIT DATA INTERRUPT\n");
			sem_status = semTake(xmit_err_sem,NO_WAIT);
			if(sem_status == OK)
			  {
			    printf("NO DAMN GOOD: ERROR = %d\n",dma_err1);
			    xmit_isr_done=0;
			    init_dma(meter,13);
			    *d0csr = 0xff;
			    taskDelay(1);
			    *d0ccr = 0xC8;  /* start DMA */
			  }
		      }
		    if(meter == 2)
		      {
			sem_status = semTake(testp_data_sem,1000); /* extend wait for graph mode which averages and draws */
			if(sem_status == OK)
			  {
			    flt_pt(0); /* Update Continuously Mode */
			    printf("FORE_TESTP_PWR = %-7.2f dBm\n",fore_testp_pwr);
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
			    init_dma(meter,13);
			    *d1csr = 0xff;
			    taskDelay(1);
			    *d1ccr = 0xC8;  /* start DMA */
			  }
		      }
		  }
		else
		  {
		    if(meter == 1)
		      {
			sem_status = semTake(xmit_data_sem,1000);
			if(sem_status == OK)
			  {
			    printf("count %d %s\n",i,(xmit_array + STANDARD_BASE));
			    xmit_isr_done=0;
			    taskDelay(1);
			    *d0ccr = 0x48;
			  }
			else
			  printf("NO XMIT DATA INTERRUPT\n");
			sem_status = semTake(xmit_err_sem,NO_WAIT);
			if(sem_status == OK)
			  {
			    printf("NO DAMN GOOD: ERROR = %d\n",dma_err1);
			    xmit_isr_done=0;
			    init_dma(meter,13);
			    *d0csr = 0xff;  /* clear status register */
			    taskDelay(1);
			    *d0ccr = 0xC8;  /* start DMA */
			  }
		      }
		    if(meter == 2)
		      {
			sem_status = semTake(testp_data_sem,1000);
			if(sem_status == OK)
			  {
			    flt_pt(0); /* Update Continuously Mode */ 
			    printf("FORE_TESTP_PWR = %-7.2f dBm\n",fore_testp_pwr);
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
			    init_dma(meter,13);
			    *d1csr = 0xff;  /* clear status register */
			    taskDelay(1);
			    *d1ccr = 0xC8;  /* start DMA */
			  }
		      }
		  }
		if(meter == 1)
		  {
		    *d0ccr = 0x10; /* Abort channel 2 DMA operation */
		    *d0csr = 0xff; /* Clear DMA status register */
		
		/* try clearing GPIB Interface */
#ifdef DESPERATE	
		    *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
		    *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
		    taskDelay(1);  /* SET DELAY */
		    *g1acr = 0x0f; /* CLEAR sic */
		
		/* handle any pending interrupts */
		
		    sem_status = semTake(xmit_data_sem,NO_WAIT);
		    sem_status = semTake(xmit_err_sem,NO_WAIT);
#endif
		  }
		if(meter == 2)
		  {
		    *d1ccr = 0x10; /* Abort channel 2 DMA operation */
		    *d1csr = 0xff; /* Clear DMA status register */
		
		/* try clearing GPIB Interface */
#ifdef DESPERATE	
		    *g2acr = 0x00; /* CLEAR SOFTWARE RESET */
		    *g2acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
		    taskDelay(1);  /* SET DELAY */
		    *g2acr = 0x0f; /* CLEAR sic */
		
		/* handle any pending interrupts */
		
		    sem_status = semTake(testp_data_sem,NO_WAIT);
		    sem_status = semTake(testp_err_sem,NO_WAIT);
#endif
		  }
	      }
	  }
      }
    talk(meter);
    printf("Another Command?\n");
    scanf(" %c",&response); 
  }while (response == 'y'); /* CHECK TO SEE IF WE WANT TO CONTINUE OR ABORT */

/* Return to Local Control */

 Return_to_Local(meter);
  
#ifdef Init
  sysIntDisable(3); /* disable IRQ3 */
#endif
  
}








