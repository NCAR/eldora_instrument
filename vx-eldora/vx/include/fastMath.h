/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 fastMath.hh
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
#ifndef INCfastMath_hh
#define INCfastMath_hh

extern "C" {
static int fastround(double d);
void saveFpcr(void);
void restoreFpcr(void);
void setRounding(void);
};

static inline int fastround(double d)
{
    register int i;

    asm volatile ("fmove%.l %1,%0" : "=d" (i) : "f" (d));

    return(i);
}


#endif // INCfastMath_hh
