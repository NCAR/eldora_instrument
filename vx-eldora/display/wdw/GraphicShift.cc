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

void GraphicController::shift(FAST Point left, FAST Point right, 
			      FAST int shift, FAST int direction = SHIFT_RIGHT)
{
    FAST void *base = baseAddr;
    FAST int xlength = right.x - left.x;
    FAST int ylength = right.y - left.y;

    
    FAST int xy = 4096 * left.y; // Start at top.

    if (direction == SHIFT_RIGHT)
      xy += right.x;
    else
      xy += left.x;

    FAST int rump = xy & 0x3;	// At long border?
    FAST int odd = (xlength - rump) % sizeof(long); // Extra bytes?

    for (FAST int i = 0; i < ylength; i++, xy += 4096)
      {
	  FAST int xl = xlength;
	  FAST unsigned char *csrc = base + xy;	// Address of next line.
	  FAST unsigned char *cdst;

	  if (direction == SHIFT_RIGHT)
	    cdst = csrc + shift;
	  else
	    cdst = csrc - shift;

	  FAST int rp;

	  if ((rp = rump))	// If we need to get to 4 byte border.
	    {
		if (direction == SHIFT_RIGHT)
		  {
		      while (rp-- && xl-- > -1)
			*cdst-- = *csrc--;
		  }
		else
		  {
		      while (rp-- && xl-- > -1)
			*cdst++ = *csrc++;
		  }
	    }
	  if (xl)		// If anything's left.
	    {
		xl -= odd;	// Get correct size for loop.

		if (xl)
		  {
		      xl /= sizeof(long); 
		      FAST long *ldst = (long *)cdst;
		      FAST long *lsrc = (long *)csrc;

		      if (direction == SHIFT_RIGHT)
			{
			    switch(xl % 10)
			      {
				  do {
				    case 0: *ldst-- = *lsrc--;
				    case 9: *ldst-- = *lsrc--;
				    case 8: *ldst-- = *lsrc--;
				    case 7: *ldst-- = *lsrc--;
				    case 6: *ldst-- = *lsrc--;
				    case 5: *ldst-- = *lsrc--;
				    case 4: *ldst-- = *lsrc--;
				    case 3: *ldst-- = *lsrc--;
				    case 2: *ldst-- = *lsrc--;
				    case 1: *ldst-- = *lsrc--;
					} while (xl -= 10 > 0);
			      }
			}
		      else
			{
			    switch(xl % 10)
			      {
				  do {
				    case 0: *ldst++ = *lsrc++;
				    case 9: *ldst++ = *lsrc++;
				    case 8: *ldst++ = *lsrc++;
				    case 7: *ldst++ = *lsrc++;
				    case 6: *ldst++ = *lsrc++;
				    case 5: *ldst++ = *lsrc++;
				    case 4: *ldst++ = *lsrc++;
				    case 3: *ldst++ = *lsrc++;
				    case 2: *ldst++ = *lsrc++;
				    case 1: *ldst++ = *lsrc++;
					} while (xl -= 10 > 0);
			      }
			}
		  }
		if (odd)	// Any thing left?
		  {
		      if (direction == SHIFT_RIGHT)
			{
			    while (odd--)
			      *cdst-- = *csrc--;
			}
		      else
			{
			    while (odd--)
			      *cdst++ = *csrc++;
			}
		  }

	    }
      }
}
