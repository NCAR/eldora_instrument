/*
 *	$Id$
 *
 *	Module:		 Flags.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 
 *
 * revision history
 * ----------------
 * $Log$
Revision 1.3  1991/09/25  20:40:34  thor
Really want binary semaphore.

Revision 1.2  90/12/27  09:48:33  thor
Modified to use new mutex semaphores and WRS ansi headers.

Revision 1.1  90/11/15  11:52:29  thor
Initial revision

 *
 * description:
 *        This is then header file for the class Flags, which
 * implements local event flags for VxWorks. By use of this class one
 * can cause a task to wait on a set of events, having it resume on
 * either the OR or AND of those events. This is especially useful for
 * time outs associated with some event.
 */
#ifndef INCFlagshh
#define INCFlagshh

#include "vxWorks.h"
#include "semLib.h"
#include "taskLib.h"
#include "stdio.h"

const int FLAGS_OR = 0;
const int FLAGS_AND = 1;

const unsigned int WD_FLAG = 0x80000000;
const unsigned int IO_FLAG = 0x40000000;

class Flags {
  protected:
    unsigned int flags;		/* The current flag settings. */
    SEM_ID flag_sem;		/* To control access and force waits. */

  public:
    Flags(void) 
      {
	  flags = 0;
	  flag_sem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
      }

    ~Flags(void)
      {
	  semTake(flag_sem,WAIT_FOREVER); /* Wait till last user is done. */
	  semDelete(flag_sem);
      }

    void set(unsigned int mask)
      {
	  flags |= mask;	/* Update flags. */
	  semGive(flag_sem);
      }

    void clear(void)
      {
	  flags = 0;
      }

    unsigned int wait(unsigned int mask, int type, int timeout = WAIT_FOREVER);

    unsigned int query(void)
      {
	  return(flags);
      }
};
		    

#endif INCFlagshh

