/*
 *	$Id$
 *
 *	Module:	RP7_START
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  16:07:52  eric
 * Initial revision
 *
 *
 * description: This module spawns the executive control module of the ELDORA 
 *              RP7 Data Acquisition function.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include "vxWorks.h"
#include "taskLib.h"
extern void dac(); 
extern void coll_data_xfer();
extern void fft_data_xfer();
go()
{
int taskid;

/* Spawn other required processes */

taskDelete(taskNameToId("texec"));
taskid =
taskSpawn("texec",10,VX_FP_TASK | VX_STDIO,8000,(FUNCPTR )dac,0,0,0,0,0,0,0,0,0,0);
if(taskid == ERROR)
  {
      printf("failed to spawn executive task! \n");
      return(-1);
  }
taskDelete(taskNameToId("treal"));
taskid =
taskSpawn("treal",5,VX_FP_TASK | VX_STDIO,3000,(FUNCPTR )coll_data_xfer,0,0,0,0,0,0,0,0,0,0);
if(taskid == ERROR)
  {
      printf("failed to spawn real-time task! \n");
      return(-1);
  }
/*
taskDelete(taskNameToId("tfft"));
taskid =
taskSpawn("tfft",150,VX_FP_TASK | VX_STDIO,3000,(FUNCPTR )fft_data_xfer,0,0,0,0,0,0,0,0,0,0);
if(taskid == ERROR)
  {
      printf("failed to spawn fft task! \n");
      return(-1);
  }
*/
}
