/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 AngleConvert.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1991/02/27  14:42:34  thor
 * *** empty log message ***
 *
 * Revision 1.2  1991/02/27  14:36:31  root
 * *** empty log message ***
 *
 * Revision 1.1  1991/02/27  14:29:02  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This header contains some simple functions to convert
 * between radians and degrees. 
 *
 */
#ifndef INCAngleConverthh
#define INCAngleConverthh

extern "C" {
#include "math.h"
};

static const double TORAD = M_PI / 180.0;
static const double TODEG = 180.0 / M_PI;

static inline double DegreesToRadians(double ang)
{
    return(ang * TORAD);
}

static inline double RadiansToDegrees(double ang)
{
    return(ang * TODEG);
}

#endif /* INCAngleConverthh */

