/*
 *	$Id$
 *
 *	Module:	SYNC	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  16:07:47  eric
 * Initial revision
 *
 *
 * description: This module provides performs a software reset of the 
 *              RP7 radar processor for the purpose of synchronization.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "RadarGbls.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

sync(f1_flag,f2_flag,f3_flag)
int  f1_flag, f2_flag, f3_flag;

{

int sem_status;

timoffb();
dspstop(1,4,0,0);
if(f1_flag)
  {
      dspstop(1,1,3,-1);
      dspstop(1,1,2,-1);
      dspstop(1,1,1,-1);
  }

if(f2_flag)
  {
      dspstop(1,2,3,-1);
      dspstop(1,2,2,-1);
      dspstop(1,2,1,-1);
  }
			
if(f3_flag)
  {
      dspstop(1,3,3,-1);
      dspstop(1,3,2,-1);
      dspstop(1,3,1,-1);
  }

dspgo(1,4,0,0);
if(f1_flag)
  {
      dspgo(1,1,3,3);
      dspgo(1,1,3,2);
      dspgo(1,1,3,1);
      dspgo(1,1,3,0);
      dspgo(1,1,2,-1);
      dspgo(1,1,1,-1);
  }
			
if(f2_flag)
  {
      dspgo(1,2,3,3);
      dspgo(1,2,3,2);
      dspgo(1,2,3,1);
      dspgo(1,2,3,0);
      dspgo(1,2,2,-1);
      dspgo(1,2,1,-1);
  }

if(f3_flag)
  {
      dspgo(1,3,3,3);
      dspgo(1,3,3,2);
      dspgo(1,3,3,1);
      dspgo(1,3,3,0);
      dspgo(1,3,2,-1);
      dspgo(1,3,1,-1);
  }
/* Clear DP Out of Sync Interrupt that may occur as a result of the 
	 stoppage  */

sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

if (sem_status == OK)
  {
      *bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            
  }
/* Clear Collator Out of Sync Interrupt that may occur as a result of 
         the stoppage */

sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
if (sem_status == OK)
  {
      *bim_cr3 = 0xd9;  /* re-enable interrupt INT3* */            
  }
taskDelay(5); /* ensure all dsp's have completed initializations */
timonb();

}

     

