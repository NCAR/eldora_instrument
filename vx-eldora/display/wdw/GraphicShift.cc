/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		  GraphicShift.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:51:14  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1991/02/27  16:56:05  thor
 * Fixed shift routine to use qpdms.
 *
 * Revision 1.1  90/12/04  10:19:53  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This method moves a rectangluar block of graphic memory. It
 * attempts to avoid overwriting data still to be moved by shifting
 * from the the right side for rightt shifts and from the left for
 * left shifts. 
 *
 * Needs to be optimized!!!!
 *
 */
#include "GraphicController.hh"

void GraphicController::shift(Point src, Point dst, FAST int width,
			      FAST int height)
{
    long data[4];
    FAST unsigned short *ptr = (unsigned short *)&data;
    FAST long *send = data;

    *ptr++ = 0x22;
    *ptr++ = width;
    *ptr++ = height;
    *ptr++ = 0x5c;
    *ptr++ = src.x;
    *ptr++ = src.y;
    *ptr++ = dst.x;
    *ptr = dst.y;

    qpdmCmd(send,4);
}
