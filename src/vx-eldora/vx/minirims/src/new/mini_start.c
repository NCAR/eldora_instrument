/*
 *	$Id$
 *
 *	Module:	MINI_START
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/02/09  18:29:47  craig
 * Initial revision
 *
 *
 * Initial revision
 *
 *
 * description: This module spawns the minirims display and data read 
 *              tasks.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define NO_MIN_SCOPE
#define OK_RPC
#include "minGbl.h"
#include "vxWorks.h"
#include "taskLib.h"
extern void mini_menu(); 
extern void mini_rd();
extern void mini_rt_rd();
extern void align_mini();

go()
{
int taskid;

/* Spawn other required processes */
/*
taskDelete(taskNameToId("tdisp"));
taskid =
taskSpawn("tdisp",80,VX_FP_TASK | VX_STDIO,8000,(FUNCPTR )mini_menu,0,0,0,0,0,0,0,0,0,0);
if(taskid == ERROR)
  {
      printf("failed to spawn display task! \n");
      return(-1);
  }
*/  
taskDelete(taskNameToId("talign"));
taskid =
taskSpawn("talign",80,VX_FP_TASK | VX_STDIO,8000,(FUNCPTR )align_mini,0,0,0,0,0,0,0,0,0,0);
if(taskid == ERROR)
  {
      printf("failed to spawn align task! \n");
      return(-1);
  }

taskDelete(taskNameToId("tread"));
tread =
taskSpawn("tread",75,VX_FP_TASK | VX_STDIO,8000,(FUNCPTR )mini_rd,0,0,0,0,0,0,0,0,0,0);
if(tread == ERROR)
  {
      printf("failed to spawn read task! \n");
      return(-1);
  }
taskSuspend(tread);

taskDelete(taskNameToId("trtread"));
trtread =
taskSpawn("trtread",75,VX_FP_TASK | VX_STDIO,8000,(FUNCPTR )mini_rt_rd,0,0,0,0,0,0,0,0,0,0);
if(trtread == ERROR)
  {
      printf("failed to spawn real-time read task! \n");
      return(-1);
  }
  
}




