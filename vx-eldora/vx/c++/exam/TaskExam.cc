/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

// Required.
#include "Task.hh"

// This section declares standard C functions that you can call from
// both C & C++. Include this in a header file so others can call
// them. You MUST use ANSI prototypes.

#ifdef __cplusplus
extern "C" {
#endif
void startme(int i);
void killme(Task *task);
#ifdef __cplusplus
};
#endif

// A local definition - only C++ code can call this.
void myLoop(Task &self, int i);

// Sample bit assignments for the flag mask. They should really be in
// a header file.
static const int WAKEUP = 1;
static const int PRINT  = 2;
static const int ZERO   = 4;
static const int DIE    = 8;

// This routine starts up another task and then communicates via flags.
void startme(int i)
{
    int args[1];		// This is a list of arguements to
				// pass to the new task. Anything may
				// be passed, if it will fit in an
				// integer.
    int argsize = 1;		// How many arguements are passed. 

    args[0] = i;

// This is the call that actually starts the task. Note the priority
// is 50. Normally you want these other tasks to run at a higher
// priority the the default. (50 is higher then 100) We use the rest
// of the defaults.
    Task *theLoop = new Task((FUNCPTR)myLoop,args,argsize,50);

// This allows the other task to get started.
    taskDelay(1);

    FAST int mask = WAKEUP;

    theLoop->SetFlags(WAKEUP);	// Send a wakeup call.

    taskDelay(1);		// Delay to simulate normal activity.

    FAST int j = 10;

    mask = PRINT;

    for (FAST int k = 0; k < j; k++) // Send a series of messages.
      {
	  theLoop->SetFlags(mask);
	  taskDelay(1);
      }

    mask = ZERO | PRINT;	// Example of dual flags.

    theLoop->SetFlags(mask);

    printf("0x%x\n",theLoop);	// Print address of Task object - for
				// use in killme.
}

void killme(Task *task)
{
    task->SetFlags(DIE);	// Die!

    delete(task);		// Required - frees memory.
}

void myLoop(FAST Task &self, FAST int i)
{
    self.FlagsInit();		// Required if you want flags.

    FAST int mask = DIE | PRINT | WAKEUP | ZERO; // This is the set of
						 // bits we will
						 // respond to, all
						 // others are ignored.

    for (;;)			// Forever.
      {
// This call waits for one of more of the bits to be set. FLAGS_AND
// would cause it to wait for ALL the bits to be set. Note we use the
// default WAIT_FOREVER timeout value.
	  FAST int flags = self.WaitOnFlags(mask,FLAGS_OR); // Wait
							    // for a
							    // bit to
							    // be set.

	  switch(flags)		// Which one?
	    {
	      case DIE:
		printf("Got me! End value is %d\n",i);
		return;
		break;

	      case PRINT:
		printf("%d\n",i);
		break;

	      case ZERO:
	      case (ZERO | PRINT): // Allows us to handle multiple
				   // sources setting flags.
		i = 0;
		printf("%d\n",i);
		break;

	      case WAKEUP:
		printf("Hello, starting value is %d\n",i);
		break;
	    }

	  i++;
      }
}

