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

#include "Task.hh"

Task::Task(FUNCPTR entry, FAST int *args, int argsize, int pri = 100,
	   int stacksize = 3000, int options = VX_FP_TASK | VX_STDIO,
	   int go = 1)
{
   
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

	  taskId = -1;		// Make sure things fail.
      }
    else
      {
	  if (argsize)
	    taskId = 
	    taskSpawn("",pri,options,stacksize,entry,(int)this,args[0],
		      args[1],args[2],args[3],args[4],args[5],args[6],
		      args[7],args[8]);
	  else
	    taskId = taskSpawn("",pri,options,stacksize,entry,(int)this);
      }

    flags = NULL;		// No flags by default.
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
    return(taskId = taskSpawn("",priority,opts,stack,start,argv[0],argv[1],
			      argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],
			      argv[8],argv[9]));
}

STATUS Task::FlagsInit(void)
{
    flags = new Flags();

    if (flags == NULL)
      return(ERROR);
    
    return(OK);
}

void Task::SetFlags(FAST unsigned int mask)
{
    if (flags == NULL)		// Protect ourself from pointer error.
      return;

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
