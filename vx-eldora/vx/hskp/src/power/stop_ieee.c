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

taskSuspend(xmit); /* stop run-time power task */
taskSuspend(testp); /* stop run-time power task */

/*********** DMA CHANNEL 0 - STOP! ***************/

  *d0csr=0xff; /* Clear channel status register by writing all ones */
  *d0ccr=0x10; /* Abort DMA operation */

/*********** DMA CHANNEL 1 - STOP! **************/

  *d1csr=0xff; /* Clear channel status register by writing all ones */
  *d1ccr=0x10; /* Abort DMA operation */
  
  /* Try clearing GPIB interface */

  *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
  *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
  taskDelay(1);  /* SET DELAY */
  *g1acr = 0x0f; /* CLEAR sic */

  *g2acr = 0x00; /* CLEAR SOFTWARE RESET */
  *g2acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
  taskDelay(1);  /* SET DELAY */
  *g2acr = 0x0f; /* CLEAR sic */

  /* handle any pending interrupts */

  sem_status = semTake(xmit_pwr_sem,NO_WAIT);
  sem_status = semTake(xmit_data_sem,NO_WAIT);
  sem_status = semTake(xmit_err_sem,NO_WAIT);
  sem_status = semTake(testp_data_sem,NO_WAIT);
  sem_status = semTake(testp_err_sem,NO_WAIT);

}



