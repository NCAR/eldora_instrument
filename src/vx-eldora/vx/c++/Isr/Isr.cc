/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		Isr.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/12/03  15:50:59  thor
 * Greatly simplified things.
 *
 * Revision 1.1  1993/11/30  17:33:31  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Isr.hh"
#include <iv.h>

// OK, until I figure out how to work around this, I use my own definition of
// intCoonect().

typedef void            (*VOIDFUNCPTR) (...);
typedef void            (*VOIDFUNCPTR_INT)(int);
extern "C" {
  STATUS intConnect(VOIDFUNCPTR *, VOIDFUNCPTR_INT, int);
};

static void isrGlue(int);

Isr::Isr(FAST int vector, FAST int useSem)
{
    if (useSem)
      sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    else
      sem = NULL;
    
    VOIDFUNCPTR *vec = (VOIDFUNCPTR *)INUM_TO_IVEC(vector);

    intConnect(vec,&isrGlue,(int)this);
}

Isr::Isr(void *vector, int useSem)
{
    if (useSem)
      sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    else
      sem = NULL;

    intConnect((VOIDFUNCPTR *)vector,&isrGlue,(int)this);
}

void Isr::IsrFunction()
{
    // This is the tricky part!

    if (sem)
      semGive(sem);
}

static void isrGlue(int i)
{
  Isr *isr = (Isr *)i;
  isr->IsrFunction();
}
