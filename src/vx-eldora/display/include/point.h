/*
 *	$Id$
 *
 *	Module:		 point.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  90/12/04  10:24:55  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        Description of an x,y cooordinate.
 *
 */
#ifndef INCpointh
#define INCpointh

struct singlePoint {
    unsigned short x;
    unsigned short y;
};

typedef struct singlePoint Point;

#endif INCpointh

