/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Flags.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  90/03/28  08:46:50  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This is the methods file for class Flags. The only
 * non-inline function is the wait function, which sleeps until the
 * requested flag(s) is set.
 *
 */
static char rcsid[] = "$Id$";

#include "Flags.hh"

// Wait loops on the local semaphore, checking the flags to see if the
// last change met the required criteria.
unsigned int Flags::wait(FAST unsigned int mask, FAST int type)
{
    FAST unsigned int flgs = flags;
    FAST unsigned int result = mask & flgs;

    if (type == FLAGS_OR || result)
      {
	  flags ^= mask;
	  return(result);
      }
    else if (mask == result)
      {
	  flags ^= mask;
	  return(result);
      }
	 
    FAST SEM_ID semaphore = flag_sem;

    for (;;)
      {
	  semTake(semaphore);	// Wait for change to flags.

	  flgs = flags;
	  result = mask & flgs;

	  if (type == FLAGS_OR)
	    {
		if (result)	// Will be true is ANY flag was set.
		  {
		      flags ^= mask; // Restore to useful mode for next call.
		      return(result);
		  }
		continue;
	    }
	  else if (result == mask) // All flags must be set.
	    {
		flags ^= mask;
		return(flgs);
	    }
      }
}
