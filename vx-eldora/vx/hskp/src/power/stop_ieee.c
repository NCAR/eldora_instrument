/*
 *	$Id$
 *
 *	Module:	stop_ieee.c	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1999/09/27  16:38:10  eric
 * added support for separate pgm task.
 *
 * Revision 1.1  1997/11/12  19:50:37  eric
 * Initial revision
 *
 * 
 *
 * description: The purpose of this module is to stop DMA and auxilliary clock
 *              interrupt for real-time task (pwr.c). 
 *              
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"
#include "semLib.h"
#include "taskLib.h"

void stop_ieee()
{
  int sem_status;

#ifdef CLOCK_ISR  
  sysAuxClkDisable(); /* disable Auxilliary Clock Interrupt */
#endif  

  sem_status = semTake(cal_sync_sem,WAIT_FOREVER); /* ensure pgm_rt has completed before a stop */


/*********** DMA CHANNEL 0 - STOP! ***************/

  *d0csr=0xff; /* Clear channel status register by writing all ones */
  *d0ccr=0x10; /* Abort DMA operation */

/*********** DMA CHANNEL 1 - STOP! **************/

  *d1csr=0xff; /* Clear channel status register by writing all ones */
  *d1ccr=0x10; /* Abort DMA operation */

  /* the following 3 lines wer placed above the DMA Channel Stops */

taskSuspend(xmit); /* stop run-time power task */
taskSuspend(testp); /* stop run-time power task */
taskSuspend(pgm); /* stop run-time power task */
  
#ifdef DESPERATE

  /* Try clearing GPIB interface */

  *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
  *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
  taskDelay(1);  /* SET DELAY */
  *g1acr = 0x0f; /* CLEAR sic */

  *g2acr = 0x00; /* CLEAR SOFTWARE RESET */
  *g2acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
  taskDelay(1);  /* SET DELAY */
  *g2acr = 0x0f; /* CLEAR sic */

#endif
  /* Return both PPMs to Local Control */

  Return_to_Local(1);
  Return_to_Local(2);

  /* handle any pending interrupts */

  sem_status = semTake(xmit_pwr_sem,NO_WAIT);
  sem_status = semTake(xmit_data_sem,NO_WAIT);
  sem_status = semTake(xmit_err_sem,NO_WAIT);
  sem_status = semTake(testp_data_sem,NO_WAIT);
  sem_status = semTake(testp_err_sem,NO_WAIT);

  /* give sync semaphore */

  semGive(cal_sync_sem);
}








