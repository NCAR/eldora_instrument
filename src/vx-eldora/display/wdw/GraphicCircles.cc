/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GraphicCircles.cc
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
 * Revision 1.1  90/12/04  10:20:26  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        These methods draw circles. Outline only circles plot the
 * points at the distance radius from the center. 
 *
 */
#include "GraphicController.hh"

void GraphicController::drawArc(Point center, Point left, Point right,
				FAST int radius, FAST unsigned char color)
{
    long data[6];
    FAST unsigned short *ptr = (unsigned short *)&data;
    FAST long *send = data;
    FAST int size = 0;

        if (color != lastColor)	// Is new color needed?
      {
	  *ptr++ = SET_COLOR;
	  *ptr++ = (unsigned short)color;
	  size++;
	  lastColor = color;
      }

    *ptr++ = ARC | lastQpdmOp;
    *ptr++ = center.x;
    *ptr++ = center.y;
    *ptr++ = radius | 0x8000;	// Use relative addressing.
    *ptr++ = radius;
    *ptr++ = left.x;
    *ptr++ = left.y;
    *ptr++ = right.x;
    *ptr++ = right.y;
    *ptr = 0;			// Pad out long with a no-op.

    if (!size)
      qpdmCmd(send,5);
    else
      qpdmCmd(send,6);

}

void GraphicController::drawCircle(FAST Point center, FAST int radius,
				   FAST unsigned char color)
{
    long data[4];
    FAST unsigned short *ptr = (unsigned short *)&data;
    FAST long *send = data;
    FAST int size = 0;

    if (color != lastColor)	// Is new color needed?
      {
	  *ptr++ = SET_COLOR;
	  *ptr++ = (unsigned short)color;
	  size++;
	  lastColor = color;
      }

    *ptr++ = CIRCLE | lastQpdmOp;
    *ptr++ = center.x;
    *ptr++ = center.y;
    *ptr++ = radius | 0x8000;	// Use relative addressing.
    *ptr++ = radius;
    *ptr = 0;			// Pad out long with a no-op.

    if (!size)
      qpdmCmd(send,3);
    else
      qpdmCmd(send,4);

}

void GraphicController::drawFilledCircle(Point center, FAST int radius,
					 FAST unsigned char color)
{
    drawCircle(center,radius,color);

    long data[3];
    FAST unsigned short *ptr = (unsigned short *)&data;
    FAST long *send = data;
    FAST int size = 0;

    *ptr++ = 0x21;		// Op code for set search color.
    *ptr++ = (unsigned short)color;
    *ptr++ = 0x54 | lastQpdmOp;	// Op code for filled bounded region.
    *ptr++ = center.x;
    *ptr++ = center.y;

    *ptr = 0;

    qpdmCmd(send,3);
}
