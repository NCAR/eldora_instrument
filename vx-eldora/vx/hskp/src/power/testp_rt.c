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
 * Revision 1.2  1999/09/27  16:42:27  eric
 * added code to restart testpulse power meter if interrupts failed.
 *
 * Revision 1.1  1997/11/12  19:51:43  eric
 * Initial revision
 *
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
		    {
		      printf("FORE TP FREQ = %d: FORE XMIT FREQ = %d\n",fore_vmehndshk->tpulse.freq_num[0],fore_vmehndshk->tpulse.freq_num[1]);	
		      printf("AFT TP FREQ = %d: AFT XMIT FREQ = %d\n",aft_vmehndshk->tpulse.freq_num[0],aft_vmehndshk->tpulse.freq_num[1]);
		      printf("FORE C =%4X: AFT C =%4X\n",fore_vmehndshk->tpulse.combined_freq_num,aft_vmehndshk->tpulse.combined_freq_num);
		      printf("FORE_TESTP_PWR = %-7.2f dBm  AFT_TESTP_PWR = %-7.2f dBm\n",fore_testp_pwr,aft_testp_pwr);	
		    }
		  fore_tp_level = fore_testp_pwr;
		  aft_tp_level = aft_testp_pwr;
		  New_tp = 0;
		  tp_cnt = 0;
		}	      tp_cnt++;
	      taskDelay(1);
	    }
	  testp_isr_done=0;
	  *d1ccr = 0x48;  /* re-enable DMA */
	}
      else
	{
	  if(autocal)
	    {
	      printf("NO TESTP DATA INTERUPT\n");
	      fore_tp_level = -999.0;
	      aft_tp_level = -999.0;
	  /* try re-starting interface */

	      *d1ccr = 0x10; /* Abort channel 2 DMA operation */
	      taskDelay(1);
	      *d1csr = 0xff; /* Clear DMA status register */

	      /* Return PPM Control to Local */
	      
	      Return_to_Local(2);
#ifdef DESPERATE	      
	  /* try clearing GPIB Interface */
	      
	      *g2acr = 0x00; /* CLEAR SOFTWARE RESET */
	      *g2acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
	      taskDelay(1);  /* SET DELAY */
	      *g2acr = 0x0f; /* CLEAR sic */          
#endif 	  
	  /* handle any pending interrupts */
	  
	      sem_status = semTake(testp_data_sem,NO_WAIT);
	      sem_status = semTake(testp_err_sem,NO_WAIT);
	      init_gpib(2); /* try this here to re-establish comms */
	      taskDelay(1);
	      strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
	      send_cmnd_string(TESTP,string_array); /* on Test Pulse meter */
	      init_dma(2,24);
	      taskDelay(2);
	      listener(2);
	      *d1csr = 0xff;  /* clear status register */
	      taskDelay(1);
	      *d1ccr = 0xC8;  /* start DMA */
	    }
	}
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







