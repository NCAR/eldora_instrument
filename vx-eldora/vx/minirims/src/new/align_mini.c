/*
 *	$Id$
 *
 *	Module:	align_mini	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/05/20  20:37:19  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  16:21:52  craig
 *
 *
 * description: This module start the miniRIMS alignment process.  Note
 *              that the miniRIMS will only be aligning when this function
 *              returns, it will not have completed the alignment process.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_MIN_SCOPE
#define STAND_ALONE
#define NO_MOTOR
#define ALIGN_POS 0.0
#include "minGbl.h"
#include "numinDef.h"

void align_mini()
{
unsigned short *min_stat;
int sem_status;

for(;;)
  {
    /* Wait for Start Semaphore */

    sem_status = semTake(align_sem,WAIT_FOREVER);
    if(sem_status == OK)
      {

	min_stat=(unsigned short *)(MIN_BASE+MIN_STATUS);

  /* Command Rotodome to Zero degrees -- i.e antennas parallel to ground */

#ifdef MOTOR
	init_motor();
	goto_pos(ALIGN_POS);
#endif

/* Cage Minirims to Zero */

	Cmd = (RUN_GYROS + ROT_GIMB_0 + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
	*Command = Cmd;

	printf("\n");
  /* Wait for GOOD Status from Minirims before Filling Structures */

	while((*min_stat & 0xF) != 0xF)
	  {
	    printf("Waiting for Minirims to Power UP; Status = %X\r",*min_stat);
	    taskDelay(100);
	  }
	printf("\nMinirims UP and Ready\n");
	semTake(align_sem,NO_WAIT); /* just in case got multiple button hits */
	Align = 1;
      }
  }
}
