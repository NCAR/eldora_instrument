/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Task.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/10/28  20:25:55  thor
 * Add destructor.
 *
 * Revision 1.1  1991/10/02  14:23:14  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This class provides an interface to the majority of VxWorks'
 * task manipulation and reporting routines. It also provides a means
 * of integrating local event flags simply. When the constructor is
 * called a task may be immediately spawned or simply readied for
 * later spawning. 
 *
 * An important note to remember is that each spawned task must have
 * as the first arguement a reference to its Task. Thus the proper
 * declaration  looks like this:
 *
 * int mytask(Task &self [rest of arg list]);
 *
 * This allows the task to use the same interface as outsiders.
 *
 * If FlagsInit() is called, the task can use local event flags. See
 * the Flags class documentation for more explanation. Be aware that
 * only the task should ever call WaitOnFlags(). If an outsider calls
 * this routine, the outsider will block!
 *
 */
#ifndef INCTaskhh
#define INCTaskhh

extern "C" {
#include "taskLib.h"
#include "sigLib.h"
#include "memLib.h"
#include "string.h"
};

#include "Flags.hh"

class Task {
  private:
    int taskId;			// Guess.

    Flags *flags;		// Local event flags, if any.

    // These are used to stored needed variables if spawning is delayed.
    FUNCPTR start;		// Entry point.
    int *argv;			// Arguement vector.
    int numArgs;		// Vector size.
    int priority;		// Guess.
    int stack;			// Stack size.
    int opts;			// Task options.
    
  public:
    Task(FUNCPTR entry, int *args, int argsize, int pri = 100,
	 int stacksize = 3000, int options = VX_FP_TASK | VX_STDIO,
	 int go = 1);

    STATUS Delete(void) { return(taskDelete(taskId)); }

    STATUS DeleteForce(void) { return(taskDeleteForce(taskId)); }

    STATUS Terminate(void) { return(taskTerminate(taskId)); }

    STATUS Suspend(void) { return(taskSuspend(taskId)); }

    STATUS Resume(void) { return(taskResume(taskId)); }

    STATUS Restart(void) { return(taskRestart(taskId)); }

    int Priority(void);

    STATUS Priority(FAST int pri) { return(taskPrioritySet(taskId,pri)); }

    STATUS Options(FAST int mask, FAST int newOpt)
      { return(taskOptionsSet(taskId, mask, newOpt)); }

    int Options(void);

    char *Name(void) { return(taskName(taskId)); }

    int Tid(void) { return(taskId); }

    BOOL IsReady(void) { return(taskIsReady(taskId)); }

    BOOL IsSuspended(void) { return(taskIsSuspended(taskId)); }

    TASK_DESC *InfoGet(void);

    WIND_TCB *Tcb(void) { return(taskTcb(taskId)); }

    char *StatusString(void);

    int Status(void);

    STATUS Exists(void) { return(taskIdVerify(taskId)); }

    int Go(void);

    STATUS SigRaise(int signal, int code = 0) 
      { return(sigRaise(taskId,signal,code)); }

    STATUS Kill(int signal) { return(SigRaise(signal)); }

    STATUS FlagsInit(void);

    void SetFlags(unsigned int mask);

    void ClearFlags(void);

    unsigned int WaitOnFlags(unsigned int mask, int type,
			     int tmo = WAIT_FOREVER);

    unsigned int QueryFlags(void);
};

#endif // INCTaskhh
