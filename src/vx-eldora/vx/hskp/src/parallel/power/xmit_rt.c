/*
 *	$Id$
 *
 *	Module: xmit_rt.c	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  2000/05/15  19:02:03  eric
 * *** empty log message ***
 *
 * Revision 1.3  1999/09/27  16:45:07  eric
 * split task into a program task (pgm_rt.c) and a data task. this is the
 * data task now.
 *
 * Revision 1.2  1999/07/13  16:49:51  thor
 * *** empty log message ***
 *
 * Revision 1.1  1997/11/12 19:45:12  eric
 * Initial revision
 *
 *
 * Initial revision
 *
 *
 * description: The purpose of this module is 
 *              handle ISRs and read xmit power
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"
#include "semLib.h"

#define NUM_SECS 5

static int isr_count;

void xmit_rt()
{
  unsigned char string_array[15];
  unsigned char number_array[11];
  unsigned char status;
  int i,prev;
  int sem_status;
  
while(1)
{
    sem_status = semTake(xmit_data_sem,1000);
    if(sem_status == OK)
    {
  	  flt_pt(1);  /* Update Now Mode */
	  if(fore_aft == 0)
	  {
	    if(Debug)
	      logMsg("FORE_XMIT_PWR=%-10.3E watts\n",fore_xmit_pwr,0,0,0,0);
	    fore_xmit_pwr = fore_xmit_pwr/1000.0; /* xmit power in kWatts */
	    aft_xmit_pwr = -999.0;
	    fore_vmehndshk->tpulse.freq_num[1] = freq_count; 
	  }
	  if(fore_aft == 1)
	  {
	    if(Debug)
	      logMsg("AFT_XMIT_PWR=%-10.3E watts\n",fore_xmit_pwr,0,0,0,0);
	    aft_xmit_pwr = fore_xmit_pwr/1000.0; /* xmit power in kWatts */
	    fore_xmit_pwr = -999.0;
	    aft_vmehndshk->tpulse.freq_num[1] = freq_count;
	  }
	  xmit_isr_done=0;
    }
    else
    {
	  if(autocal)
	  {
	    printf("NO XMIT DATA INTERUPT\n");
	    fore_xmit_pwr = -999.0;
	    aft_xmit_pwr = -999.0;
	  }
    }
    sem_status = semTake(xmit_err_sem,NO_WAIT);
    if(sem_status == OK)
    {
	  if(Debug)
	    printf("XMIT ERROR: %d \n",dma_err1);
	  xmit_isr_done=0;
	  init_dma(1,15);
	  *d0csr = 0xff;
	  taskDelay(1);
	  *d0ccr = 0xC8;  /* start DMA */
    }
  }
}

#ifdef CLOCK_ISR
void xmit_pwr_isr()
{
  isr_count++;
  if(isr_count == NUM_SECS)
    {
      semGive(xmit_pwr_sem);
      isr_count = 1;
    }
}
#endif






