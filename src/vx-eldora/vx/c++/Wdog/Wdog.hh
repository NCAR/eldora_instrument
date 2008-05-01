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
 * Revision 1.3  1993/07/29  18:30:37  thor
 * Fixed type of code to match new prototype.
 *
 * Revision 1.2  1993/07/28  17:09:20  thor
 * Upgraded tp VxWorks 5.1.
 *
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

    FUNCPTR code;		// The function to invoke when the
				// watchdog triggers.

    int parameter;		// Value passed to code.

    int delay;			// Timeout in ticks.

  public:
    Wdog(FUNCPTR func, int time, int param, int go = 0)
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
