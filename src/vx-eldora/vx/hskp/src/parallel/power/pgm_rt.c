/*
 *	$Id$
 *
 *	Module: pgm_rt.c	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  2000/05/15  18:45:04  eric
 * Removed need to reset GPIB between listen and talk transitions, also added
 * sync semaphore support for cleaner stops.
 *
 * Revision 1.1  1999/09/27  16:35:45  eric
 * Initial revision
 *
 *
 * Initial revision
 *
 *
 * description: The purpose of this module is to update power meter
 *              graphical displays and cursor position 
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"
#include "semLib.h"

#define NUM_SECS 5

static int isr_count;

void pgm_rt()
{
  unsigned char string_array[15], status;
  unsigned char number_array[11];
  int i,prev, cmnd_stat;
  int sem_status;


/************ INITIALIZE STRING AND NUMBER ARRAYS *************/

  for(i=0; i<11; i++)
    {
        number_array[i]=0;
    }
  for(i=0; i<15; i++)
    {
        string_array[i]=0;
    }
  while(1)
    {
    /* Transmit Power Meter */

      sem_status = semTake(xmit_pwr_sem,20);
      if(sem_status == OK)
	{
	/* change Power Meter curser position to point to the next freq */
	/* Also switch between Fore and Aft at appropriate time */
	
	  *d0ccr = 0x10; /* Abort channel 1 DMA operation */
	  taskDelay(1);
	  *d0csr = 0xff; /* Clear DMA status register */
	  
#ifdef DESPERATE

	  /* try clearing GPIB Interface */
	  
	  *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
	  *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
	  taskDelay(1);  /* SET DELAY */
	  *g1acr = 0x0f; /* CLEAR sic */          
 	  
	  /* handle any pending interrupts */
	  
	  sem_status = semTake(xmit_data_sem,NO_WAIT);
	  sem_status = semTake(xmit_err_sem,NO_WAIT);
	  init_gpib(1); /* try this here to re-establish comms */
#endif
	  
	  talk(1);

	  if(freq_count > fraddes->num_freq_trans)
	      freq_count = 1;

	  xmit_csr_delay = ((0.5 + freq_count-1) * (float)wave->chip_width[0] + (float)wave->chip_offset[0])/ 60.0; /* xmit pulsewidth in microseconds */

	  if(fore_aft == 1)
	    {
	      strncpy(string_array,"GRFA\0",5);     /* Select graph mode */  
	      cmnd_stat = send_cmnd_string(XMIT,string_array);
	      if(cmnd_stat == -1)
		{
		  status = ReadStatusByte(1);
		  printf("SRQ: %2X; Command: %s\n",status,string_array);
		}
	      strncpy(string_array,"FREQ\0",5); /* Select frequency */
	      sprintf(number_array,"%7.2f",fraddes->freq1); /* Use freq 1 as nominal */
	      strncat(string_array,number_array,strlen(number_array));
	      cmnd_stat = send_cmnd_string(XMIT,string_array);
	      if(cmnd_stat == -1)
		{
		  status = ReadStatusByte(1);
		  printf("SRQ: %2X; Command: %s\n",status,string_array);
		}
	      
	      strncpy(string_array,"CDLA\0",5); /*Set cursor delay for fore xmit meter */  
	      sprintf(number_array,"%9.2f",xmit_csr_delay);
	      strncat(string_array,number_array,strlen(number_array));
	      cmnd_stat = send_cmnd_string(XMIT,string_array);
	      if(cmnd_stat == -1)
		{
		  status = ReadStatusByte(1);
		  printf("SRQ: %2X; Command: %s\n",status,string_array);
		}	      
	    }
	  if(fore_aft == 0)
	    {
	      strncpy(string_array,"GRFB\0",5);    /* Select graph mode */  
	      cmnd_stat = send_cmnd_string(XMIT,string_array);
	      if(cmnd_stat == -1)
		{
		  status = ReadStatusByte(1);
		  printf("SRQ: %2X; Command: %s\n",status,string_array);
		}	      
	      
	      strncpy(string_array,"FREQ\0",5); /* Select frequency */
	      sprintf(number_array,"%7.2f",araddes->freq1); /* Use freq 1 as nominal */
	      strncat(string_array,number_array,strlen(number_array));
	      cmnd_stat = send_cmnd_string(XMIT,string_array);
	      if(cmnd_stat == -1)
		{
		  status = ReadStatusByte(1);
		  printf("SRQ: %2X; Command: %s\n",status,string_array);
		}	      
	      
	      strncpy(string_array,"CDLB\0",5); /*Set cursor delay for aft xmit meter */  
	      sprintf(number_array,"%9.2f",xmit_csr_delay);
	      strncat(string_array,number_array,strlen(number_array));
	      cmnd_stat = send_cmnd_string(XMIT,string_array);
	      if(cmnd_stat == -1)
		{
		  status = ReadStatusByte(1);
		  printf("SRQ: %2X; Command: %s\n",status,string_array);
		}	      
	      
	      freq_count++;	      
	    }
	  prev = fore_aft;
	  fore_aft = !prev;
	  strncpy(string_array,"UPDT\0",5); /* Select update trigger reset mode */ 
	  cmnd_stat = send_cmnd_string(XMIT,string_array); /* on Xmit meter */
	  if(cmnd_stat == -1)
	    {
	      status = ReadStatusByte(1);
	      printf("SRQ: %2X; Command: %s\n",status,string_array);
	    }	      
	  
	  strncpy(string_array,"UPDN\0",5); /* Select update now mode */ 
	  cmnd_stat = send_cmnd_string(XMIT,string_array); /* on Xmit meter */
	  if(cmnd_stat == -1)
	    {
	      status = ReadStatusByte(1);
	      printf("SRQ: %2X; Command: %s\n",status,string_array);
	    }	      

	  taskDelay(1);
	  init_dma(1,15);                         
	  taskDelay(1);
	  listener(1);      /* turn MZ7500 into listener and power meter into tallker */
	  *d0csr = 0xff;
	  taskDelay(1);
	  *d0ccr = 0xC8;  /* start DMA */
	}
      semGive(cal_sync_sem); /* set cal_sync semiphore for stop synchronization */
    }
}










