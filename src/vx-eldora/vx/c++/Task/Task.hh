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
 * Revision 1.9  1993/07/28  19:18:11  thor
 * UpGraded to VxWorks 5.1.
 *
 * Revision 1.8  1992/12/14  23:12:22  vanandel
 * added name field for deferred creation
 *
 * Revision 1.7  1992/12/07  17:32:12  vanandel
 * on task destruction, correctly delete the flags to allow
 * the task destruction to complete
 *
 * Revision 1.6  1992/11/24  18:51:43  vanandel
 * added destructor - delete allocated flags
 *
 * Revision 1.5  1992/10/02  13:39:30  vanandel
 * added method to return process's flags
 *
 * Revision 1.4  1992/10/02  13:24:06  vanandel
 * added optional task name
 *
 * Revision 1.3  1991/10/30  15:23:43  thor
 * Undid destructor.
 *
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

#include "taskLib.h"
#include "sigLib.h"
#include "memLib.h"
#include "string.h"

#include "Flags.hh"

class Task {
  private:
    int taskId;			// Guess.

    Flags *flags;		// Local event flags, if any.

    // These are used to stored needed variables if spawning is delayed.
    FUNCPTR start;		// Entry point.
    int argv[10];			// Argument vector.
    int numArgs;		// Vector size.
    int priority;		// Guess.
    int stack;			// Stack size.
    int opts;			// Task options.
    char *name;			// Task Name
    
  public:
    Task(FUNCPTR entry, int *args, int argsize, int pri = 100,
	 int stacksize = 3000, int options = VX_FP_TASK | VX_STDIO,
	 int go = 1, char *name = "");

    ~Task(void) {flags->set(0x1);delete flags;};

    STATUS Delete(void) { return(taskDelete(taskId)); }

    STATUS DeleteForce(void) { return(taskDeleteForce(taskId)); }

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

    int Kill(int signal) { return(kill(taskId,signal)); }

    STATUS FlagsInit(void);

    void SetFlags(unsigned int mask);

    void ClearFlags(void);

    unsigned int WaitOnFlags(unsigned int mask, int type,
			     int tmo = WAIT_FOREVER);

    unsigned int QueryFlags(void);

    // since may only want to give another process the right to manipulate
    // our flags, and not change the other state of our Task , we provide 
    // an method that only lets outsiders access our flags

    Flags *TaskFlags(void) { return (flags);}

};

#endif // INCTaskhh