/*
 *	$Id$
 *
 *	Module:	testp_rt.c	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 * description: The purpose of this module is to update test pulse
 *              power meter readings.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"
#include "semLib.h"

#define NUM_SECS 5

static int isr_count;

void testp_rt()
{
  unsigned char string_array[15];
  unsigned char number_array[11];
  int i,prev;
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
      sem_status = semTake(testp_data_sem,1000);
      if(sem_status == OK)
	{
	  if(New_tp)
	    {
	      if(tp_cnt == 5)  /* wait until test pulse has settled to record reading */
		{
		  flt_pt(0);  
		  if(Debug)
		    printf("FORE_TESTP_PWR = %-7.2f dBm  AFT_TESTP_PWR = %-7.2f dBm\n",fore_testp_pwr,aft_testp_pwr);	
		  fore_tp_level = fore_testp_pwr;
		  aft_tp_level = aft_testp_pwr;
		  New_tp = 0;
		  tp_cnt = 0;
		}
	      tp_cnt++;
	      taskDelay(1);
	    }
	  testp_isr_done=0;
	  *d1ccr = 0x48;  /* re-enable DMA */
	}
      else
	printf("NO TESTP DATA INTERUPT\n");
      sem_status = semTake(testp_err_sem,NO_WAIT);
      if(sem_status == OK)
	{
	  if(Debug)
	    printf("TESTP ERROR: %d\n",dma_err2);
	  testp_isr_done=0;
	  init_dma(2,24);
	  *d1csr = 0xff;  /* clear status register */
	  taskDelay(1);
	  *d1ccr = 0xC8;  /* start DMA */
	}
    }
}







