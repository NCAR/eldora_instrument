/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Isr.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/11/30  17:33:35  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This class sets up an ISR for a given interrupt vector. When
 * the interrupt occurs the user supplied method is called, which
 * defaults to Isr::IsrFunction(). This method simply checks to see if
 * a semaphore is in use & if so it gives the semaphore. There is a
 * trick to all of this however. We relie on the fact that VxWorks
 * passes an int sized parameter to the ISR function & the fact that
 * currently gcc interprets the top of the call stack as the 'this'
 * pointer. Therefore we can pretend that the ISR method is a regular
 * C++ method.
 * 
 */
#ifndef INCIsr_hh
#define INCIsr_hh

#include <vxWorks.h>
#include <intLib.h>
#include <semLib.h>

class Isr {
  public:
    Isr(int vector, int useSem = 0, VOIDFUNCPTR isr = NULL);
    Isr(void *vector, int useSem = 0, VOIDFUNCPTR isr = NULL);

    STATUS IsrWait(int tmo = WAIT_FOREVER) { semTake(sem,tmo); }
  protected:
    SEM_ID sem;

    void IsrFunction();
};


#endif // INCIsr_hh
