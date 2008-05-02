/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Task.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.8  1993/07/28  19:18:11  thor
 * UpGraded to VxWorks 5.1.
 *
 * Revision 1.7  1992/12/07  17:27:51  vanandel
 * move create of flags so it occurs, whether or not the task is created
 * immediately
 *
 * Revision 1.6  1992/10/02  13:26:43  vanandel
 * added include to declare type of logMsg()
 *
 * Revision 1.5  1992/10/02  13:23:35  vanandel
 * added optional task name
 *
 * Revision 1.4  1992/10/02  13:16:55  vanandel
 * made logMsg's conditionally compiled
 *
 * Revision 1.3  1992/10/01  00:16:34  vanandel
 * always use flags, add some debug messages
 *
 * Revision 1.2  1992/06/24  15:58:25  thor
 * Fixed constructor to allow for 0 length arg list and immediate start.
 *
 * Revision 1.1  1991/10/02  14:23:10  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This file has methods for construction, some reporting
 * methods and the flag methods.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "logLib.h"
#include "Task.hh"
#include "stdioLib.h"

Task::Task(FUNCPTR entry, FAST int *args, int argsize, int pri = 100,
	   int stacksize = 3000, int options = VX_FP_TASK | VX_STDIO,
	   int go = 1, char *theName = "")
{
   
    flags = new Flags();
    if (flags == NULL)
      {
	  logMsg("couldn't allocate flags for task %x\n", taskId,0,0,0,0,0);
      }
    if (!go)
      {
	  start = entry;

	  priority = pri;

	  FAST int *in = args;
	  FAST int *out = argv;


	  *out++ = (int)this;	// We must know who we are!

	  for (FAST int i = 1; i <= argsize; i++)
	    *out++ = *in++;

	  numArgs = argsize;

	  stack = stacksize;
	  
	  opts = options;

	  name = theName;

	  taskId = ERROR;		// Make sure things fail.
      }    else
      {
	  if (argsize)
	    taskId = 
	    taskSpawn(theName,pri,options,stacksize,entry,(int)this,args[0],
		      args[1],args[2],args[3],args[4],args[5],args[6],
		      args[7],args[8]);
	  else
	    taskId = taskSpawn(theName,pri,options,stacksize,entry,(int)this,
			       0,0,0,0,0,0,0,0,0);
      }

}

int Task::Priority(void)
{
    int pri;

    if (taskPriorityGet(taskId,&pri) == ERROR)
      return(ERROR);

    return(pri);
}

int Task::Options(void)
{
    int opt;

    if (taskOptionsGet(taskId,&opt) == ERROR)
      return(ERROR);

    return(opt);
}

TASK_DESC *Task::InfoGet(void)
{
    FAST TASK_DESC *ptr = (TASK_DESC *)malloc(sizeof(TASK_DESC));

    if (ptr == NULL)
      return(ptr);

    if (taskInfoGet(taskId,ptr) == ERROR)
      return(NULL);

    return(ptr);
}

char *Task::StatusString(void)
{
    FAST char *ptr = new char[20];

    if (ptr == NULL)
      return(ptr);

    if (taskStatusString(taskId,ptr) == ERROR)
      return(NULL);

    return(ptr);
}
	
int Task::Status(void)
{
    FAST WIND_TCB *tcb = taskTcb(taskId);

    if (tcb == NULL)
      return(-1);

    return(tcb->status);
}

int Task::Go(void)
{
    return(taskId = taskSpawn(name,priority,opts,stack,start,argv[0],argv[1],
			      argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],
			      argv[8],argv[9]));
}

STATUS Task::FlagsInit(void)
{
#ifdef DEBUG
    logMsg("Task::FlagsInit called this = %x, flags = %x \n", this, flags);
#endif

    if (flags == NULL)
      return(ERROR);
    
    return(OK);
}

void Task::SetFlags(FAST unsigned int mask)
{
    if (flags == NULL)		// Protect ourself from pointer error.
    {
#ifdef DEBUG
      logMsg("Task::SetFlags called this = %x, with NIL flags\n", this);
#endif
      return;
    }

#ifdef DEBUG
    logMsg("Task::SetFlags called\n");
#endif
    flags->set(mask);
}

void Task::ClearFlags(void)
{
    if (flags == NULL)		// Protect ourself from pointer error.
      return;

    flags->clear();
}

// The next two methods assume that no one would ever want to use all flags.

unsigned int Task::WaitOnFlags(FAST unsigned int mask, FAST int type,
			       FAST int tmo = WAIT_FOREVER)
{
    if (flags == NULL)		// Protect ourself from pointer error.
      return(0xffffffff);

    return(flags->wait(mask,type,tmo));
}

unsigned int Task::QueryFlags(void)
{
    if (flags == NULL)		// Protect ourself from pointer error.
      return(0xffffffff);

    return(flags->query());
}