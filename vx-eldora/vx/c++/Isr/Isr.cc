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
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Isr.hh"
#include <iv.h>

Isr::Isr(FAST int vector, FAST int useSem)
{
    if (useSem)
      sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    else
      sem = NULL;
    
    VOIDFUNCPTR *vec = (VOIDFUNCPTR *)INUM_TO_IVEC(vector);

    intConnect(vec,(VOIDFUNCPTR)Isr::callback,(int)this);
}

Isr::Isr(void *vector, int useSem)
{
    if (useSem)
      sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    else
      sem = NULL;

    intConnect((VOIDFUNCPTR *)vector,(VOIDFUNCPTR)Isr::callback,(int)this);
}

void Isr::IsrFunction()
{
    if (sem)
      semGive(sem);
}

void Isr::callback(Isr *callme)
{
    callme->IsrFunction();
}
