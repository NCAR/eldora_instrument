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

#include "vxWorks.hh"
#include "semLib.hh"
#include "taskLib.hh"

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
	  flag_sem = semCreate();
      }

    ~Flags(void)
      {
	  semTake(flag_sem);	/* Wait till last user is done. */
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

    unsigned int wait(unsigned int mask, int type);

    unsigned int query(void)
      {
	  return(flags);
      }
};
		    

#endif INCFlagshh

