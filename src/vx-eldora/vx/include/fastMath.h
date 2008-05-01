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
 * Revision 1.1  1993/12/09  20:31:24  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCfastMath_hh
#define INCfastMath_hh

#ifdef __cplusplus
extern "C" {
#endif

static int fastround(double d);
void saveFpcr(void);
void restoreFpcr(void);
void setRounding(void);

#ifdef __cplusplus
};
#endif

static inline int fastround(double d)
{
    register int i;

    asm volatile ("fmove%.l %1,%0" : "=d" (i) : "f" (d));

    return(i);
}


#endif // INCfastMath_hh
