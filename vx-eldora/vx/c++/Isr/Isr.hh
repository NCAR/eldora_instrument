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
 *
 *
 * description:
 *        
 */
#ifndef INCIsr_hh
#define INCIsr_hh

#include <vxWorks.h>
#include <intLib.h>
#include <semLib.h>

class Isr {
  public:
    Isr(int vector, int useSem = 0);
    Isr(void *vector, int useSem = 0);

    STATUS IsrWait(int tmo = WAIT_FOREVER) { semTake(sem,tmo); }

  protected:
    SEM_ID sem;

    virtual void IsrFunction();

  private:
    static void callback(Isr *callme);
};


#endif // INCIsr_hh
