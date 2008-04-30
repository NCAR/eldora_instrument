/*
 * $Id$
 *
 * Module: init_vmevme  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 * revision history
 * ----------------
 * Revision 1.1  1992/08/14  20:44:35  craig
 * Initial revision
 *
 *
 * description: This module initializes the vme to vme interfaces and their
 *              handshake areas 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile: init_vmevme.c,v $ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void init_vmevme()
{
/* Define some general purpose variables */
long i;


/* Setup all of the pointers to the VME to VME interface areas */

fore_vmehndshk = (struct vmevmehndshk *)(FORE_STAND_START + STANDARD_BASE); 
aft_vmehndshk = (struct vmevmehndshk *)(AFT_STAND_START + STANDARD_BASE); 

fore_local_status = (char *)(LOCAL_STATUS_REG + FORE_SHORT_START + SHORT_BASE);
fore_local_command = (char *)(LOCAL_COMMAND_REG + FORE_SHORT_START + SHORT_BASE);
fore_remote_status = (char *)(REMOTE_STATUS_REG + FORE_SHORT_START + SHORT_BASE);
fore_remote_command = (char *)(REMOTE_COMMAND_REG + FORE_SHORT_START + SHORT_BASE);
aft_local_status = (char *)(LOCAL_STATUS_REG + AFT_SHORT_START + SHORT_BASE);
aft_local_command = (char *)(LOCAL_COMMAND_REG + AFT_SHORT_START + SHORT_BASE);
aft_remote_status = (char *)(REMOTE_STATUS_REG + AFT_SHORT_START + SHORT_BASE);
aft_remote_command = (char *)(REMOTE_COMMAND_REG + AFT_SHORT_START + SHORT_BASE);

/* Clear the fore and aft test pulse handshake areas */

for(i=0; i<0x100000; i++)
  fore_vmehndshk->salute[i] = 0;
for(i=0; i<0x100000; i++)
  aft_vmehndshk->salute[i] = 0;

/* Clear the fore and aft sweep numbers to zero */
fore_sweep_num = 0;
aft_sweep_num = 0;

/* Attach the vmevme_isr to the proper interrupt vector */
if(intConnect((VOIDFUNCPTR *)(VME_VME_VEC * 4),
          (VOIDFUNCPTR)vmevme_isr,0) == ERROR)
  {
      fprintf(stderr,"failed to connect interrupt vector %d\n",VME_VME_VEC);
      exit(1);
  }
/* Create the semaphore. */
vmeSem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
/* Now spawn the attached task. */
taskSpawn("midbeam",50,VX_FP_TASK,10000,(FUNCPTR)midbeam,0,0,0,0,0,0,0,0,0,0);
}
