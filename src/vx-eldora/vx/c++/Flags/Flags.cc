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
 * Revision 1.5  1991/09/25  20:42:22  thor
 * Change of rcs versions!
 *
 * Revision 1.5  1991/09/25  20:42:22  thor
 * Change of rcs versions!
 *
 * Revision 1.4  90/12/27  09:48:00  thor
 * Modified to use new semaphore syntax.
 * 
 * Revision 1.3  90/12/07  13:48:46  thor
 * Fixed bug in test for already set flags.
 * 
 * Revision 1.2  90/11/15  11:51:11  thor
 * Added proper code to test if flags are already set. Changed code to clear
 * effected flags on success. Changed header to .hh.
 * 
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
unsigned int Flags::wait(FAST unsigned int mask, FAST int type,
			 FAST int timeout)
{
    FAST unsigned int flgs = flags;
    FAST unsigned int result = mask & flgs;

    if ((type == FLAGS_OR) && result)
      {
	  flags ^= result;
	  return(result);
      }
    else if (mask == result)
      {
	  flags ^= result;
	  return(result);
      }
	 
    FAST SEM_ID semaphore = flag_sem;

    for (;;)
      {
	  if (semTake(semaphore,timeout) == ERROR) // If we timeout,
						   // no flags were set.
	    return(0);

	  flgs = flags;
	  result = mask & flgs;

	  if (type == FLAGS_OR)
	    {
		if (result)	// Will be true is ANY flag was set.
		  {
		      flags ^= result; // Restore to useful mode for next call.
		      return(result);
		  }
		continue;
	    }
	  else if (result == mask) // All flags must be set.
	    {
		flags ^= result;
		return(flgs);
	    }
      }
}
