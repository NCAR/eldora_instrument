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

Isr::Isr(FAST int vector, FAST int useSem, VOIDFUNCPTR isr)
{
    if (useSem)
      sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    else
      sem = NULL;
    
    VOIDFUNCPTR *vec = (VOIDFUNCPTR *)INUM_TO_IVEC(vector);

    if (isr == NULL)
      isr = (VOIDFUNCPTR)&Isr::IsrFunction;
    
    intConnect(vec,isr,(int)this);
}

Isr::Isr(void *vector, int useSem, VOIDFUNCPTR isr)
{
    if (useSem)
      sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    else
      sem = NULL;

    if (isr == NULL)
      isr = (VOIDFUNCPTR)&Isr::IsrFunction;

    intConnect((VOIDFUNCPTR *)vector,isr,(int)this);
}

void Isr::IsrFunction()
{
    // This is the tricky part!

    if (sem)
      semGive(sem);
}
