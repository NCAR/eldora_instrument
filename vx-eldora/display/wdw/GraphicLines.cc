/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		  GraphicLines.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        This is the method for drawing a single color line. It uses
 * Bresnahan's algorithm. 
 *
 */
#define GRAPHIC_CTLR_PRIVATE
#include "GraphicController.hh"

void GraphicController::drawLine(Point start, Point end, 
				 FAST unsigned char color)
{
    volatile FAST unsigned char *busy = fifoFlags;
    volatile FAST long *fifo = (long *)qpdmFifo;
    long data[4];
    FAST unsigned short *ptr = (unsigned short *)&data;
    FAST long *send = data;
    FAST int size;

    if (color != lastColor)	// Is new color needed?
      {
	  *ptr++ = SET_COLOR;
	  *ptr++ = (unsigned short)color;
	  size++;
	  lastColor = color;
      }

    *ptr++ = LINE | lastQpdmOp;
    *ptr++ = start.x;
    *ptr++ = start.y;
    *ptr++ = end.x;
    *ptr++ = end.y;
    *ptr = 0;			// Pad out long with a no-op.

    semTake(accessSem);

    while (!(*busy & (FIFO_HF | FIFO_AE))); // If either is set there
					     // is at least 256 bytes left.

    *fifo = *send++;
    *fifo = *send++;
    *fifo = *send++;

    if (size)
      *fifo = *send;

    semGive(accessSem);
}
