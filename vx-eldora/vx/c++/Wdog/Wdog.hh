/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Wdog.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/01/16  20:25:18  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This class encapulates the WRS watchdog functions.
 *
 */
#ifndef INCWdoghh
#define INCWdoghh

#include "vxWorks.h"
#include "wdLib.h"

class Wdog {
  private:
    WDOG_ID wid;		// The watchdog id.

    VOIDFUNCPTR code;		// The function to invoke when the
				// watchdog triggers.

    int parameter;		// Value passed to code.

    int delay;			// Timeout in ticks.

  public:
    Wdog(VOIDFUNCPTR func, int time, int param, int go = 0)
      {
	  delay = time;
	  code = func;
	  parameter = param;

	  wid = wdCreate();

	  if (go)		// Default is to wait for explicit start.
	    Start();
      }

    void Start(void) { wdStart(wid,delay,code,parameter); }

    void Cancel(void) { wdCancel(wid); }

    void Restart(void) { Cancel(); Start(); }

    ~Wdog(void) { wdDelete(wid); }
};

#endif // INCWdoghh
