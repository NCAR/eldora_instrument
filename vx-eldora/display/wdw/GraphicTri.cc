/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        This code draws either an outlined triangle or a filled
 * triangle.
 *
 */
#define GRAPHIC_CTLR_PRIVATE
#include "GraphicController.hh"

void GraphicController::drawTri(Point one, Point two, Point three, FAST
				unsigned char color)
{
    drawLine(one,two,color);
    drawLine(two,three,color);
    drawLine(three,one,color);
}

void GraphicController::drawFilledTri(Point one, Point two, Point three, FAST
				      unsigned char color)
{
    long data[5];
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

    *ptr++ = FILLED_TRI | lastQpdmOp;
    *ptr++ = one.x;
    *ptr++ = one.y;
    *ptr++ = two.x;
    *ptr++ = two.y;
    *ptr++ = three.x;
    *ptr++ = three.y;
    *ptr = 0;			// Pad out long with a no-op.

    if (!size)
      qpdmCmd(send,4);
    else
      qpdmCmd(send,5);
}

