/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		  GraphicRects.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  90/12/04  10:20:17  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        These methods draw either an outlined rectangle or a filled
 * rectangle. Since they are not intended to be used for fast display
 * items, we call drawLine to do the work.
 *
 */
#define GRAPHIC_CTLR_PRIVATE
#include "GraphicController.hh"

void GraphicController::drawRect(Point left, Point right, unsigned char color)
{
    Point a;
    
    a.x = right.x;
    a.y = left.y;
    
    drawLine(left,a,color);	// Top.
    drawLine(right,a,color);	// Right side.

    a.x = left.x;
    a.y = right.y;

    drawLine(left,a,color);	// Left side.
    drawLine(right,a,color);	// Bottom.
}

void GraphicController::drawFilledRect(FAST Point left, FAST Point right, 
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

    *ptr++ = FILLED_RECT | lastQpdmOp;
    *ptr++ = left.x;
    *ptr++ = left.y;
    *ptr++ = right.x;
    *ptr++ = right.y;
    *ptr = 0;			// Pad out long with a no-op.

    if (!size)
      qpdmCmd(send,3);
    else
      qpdmCmd(send,4);
}
