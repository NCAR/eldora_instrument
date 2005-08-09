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
#include "vxWorks.h"
#include "taskLib.h"
extern void mini_menu(); 
extern void mini_rd();

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
taskDelete(taskNameToId("tread"));
taskid =
taskSpawn("tread",75,VX_FP_TASK | VX_STDIO,8000,(FUNCPTR )mini_rd,0,0,0,0,0,0,0,0,0,0);
if(taskid == ERROR)
  {
      printf("failed to spawn read task! \n");
      return(-1);
  }  
}




