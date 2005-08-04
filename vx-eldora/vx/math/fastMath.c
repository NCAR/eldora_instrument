/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 fastMath.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/12/09  20:31:19  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "fastMath.h"

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif
static int fpcrState;

void saveFpcr()
{
    register int cm;

    asm volatile ("fmove%.l fpcr,%0" : "=dm" (cm) : );

    fpcrState = cm;
}

void restoreFpcr()
{
    register int cm = fpcrState;
    
    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (cm));
}

void setRounding()
{
    register int cm = fpcrState;

    cm |= 16;
    cm &= 0xffffffdf;

    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (cm));
}

double degreeToRadians(double d)
{
    return(d * (M_PI / 180.0));
}
