/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Clock.cc
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:53:12  thor
 * First offical ELDORA release!
 *
 * Revision 1.3  1991/05/06  15:17:03  thor
 * Changed from 8x16 characters to 12x24 characters. Some minor
 * optimization.
 *
 * Revision 1.2  1991/05/03  18:05:57  thor
 * Added comments, fixed incorrect call to copyBlock.
 *
 * Revision 1.1  1991/05/03  15:19:17  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This contains the methods for the Clock class.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#pragma implementation

#include "Clock.hh"
#include "ClockP.hh"

Clock::Clock(GraphicController *cntlr, int window, unsigned short x, unsigned
	     short y, unsigned short xoff, unsigned short yoff) : 
	     Window(cntlr, window, x, y, Clock::WIDTH, Clock::HEIGHT,
		    xoff, yoff)
{
    fore = WHITE1;		// Default colors are white on black.
    back = BLACK;

    lastHour = -1;		// Force redraw on first update.
    lastMin = -1;
    lastSec = -1;

    FAST Point *ptr = location;

    FAST int j = 8;
    FAST unsigned short k = 4 + Clock::CHAR_WIDTH;
    FAST unsigned short lx = 4;	// 4 pixel wide boarders.
    FAST unsigned short ly = 4;

    for (FAST int i = 0; i < j; i++, ptr++, lx += k)
      {
	  ptr->x = lx;
	  ptr->y = ly;
      }

    j = 11;

    lx = 0;
    ly = Clock::CHAR_HEIGHT + 8; // Start data storage directly below
				 // displayed area.

    ptr = patterns;

    for (i = 0; i < j; i++, ptr++, lx += Clock::CHAR_WIDTH)
      {
	  ptr->x = lx;
	  ptr->y = ly;
      }

    setDefPatterns();

    setPriority(MAXWDWPRI);
}

void Clock::setDefPatterns(void)
{
    FAST Point *ptr = patterns;

    pattern(ptr++,ZeroMap);
    pattern(ptr++,OneMap);
    pattern(ptr++,TwoMap);
    pattern(ptr++,ThreeMap);
    pattern(ptr++,FourMap);
    pattern(ptr++,FiveMap);
    pattern(ptr++,SixMap);
    pattern(ptr++,SevenMap);
    pattern(ptr++,EightMap);
    pattern(ptr++,NineMap);
    pattern(ptr,ColonMap);

    // Clear displayed area and draw colons.
    Point a;
    a.x = 0;
    a.y = 0;
    frect(a,Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
    shift(patterns[10],location[2],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT);
    shift(patterns[10],location[5],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT);
}

void Clock::pattern(FAST Point *ptr, FAST unsigned short *pattern)
{
    Point pt = *ptr;		// Where is data space to draw character.

    FAST int j = Clock::CHAR_HEIGHT;
    FAST int l = Clock::CHAR_WIDTH;

    // Loop drawing each line.
    for (FAST int i = 0; i < j; i++, pattern++)
      {
	  FAST unsigned short p = *pattern;

	  p <<= 4;		// Get rid of unused top end nibble.

	  for (FAST int k = 0; k < l; k++)
	    {
		if ((p & 0x8000))	// Set bit means draw foreground color.
		  point(pt,fore);
		else
		  point(pt,back);

		p <<= 1;

		pt.x += 1;
	    }
	  pt.x = ptr->x;	// Get ready for start of next line.
	  pt.y += 1;
      }
}

void Clock::update(FAST short hour, FAST short minute, FAST short second)
{
    if (second != lastSec)	// Only perform as needed.
      {
	  lastSec = second;

	  FAST short i = second;

	  if (i > 9)		// Get the tens index.
	    {
		i /= 10;

		second -= (i * 10); // Get correct ones index.
	    }
	  else
	    i = 0;
	  
	  frect(location[6],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
	  shift(patterns[i],location[6],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT);
	  
	  frect(location[7],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
	  shift(patterns[second],location[7],Clock::CHAR_WIDTH,
		Clock::CHAR_HEIGHT);
      }

    if (minute != lastMin)
      {
	  lastMin = minute;

	  FAST short i = minute;

	  if (i > 9)
	    {
		i /= 10;
		minute -= (i * 10);
	    }
	  else
	    i = 0;

	  frect(location[3],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
	  shift(patterns[i],location[3],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT);

	  frect(location[4],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
	  shift(patterns[minute],location[4],Clock::CHAR_WIDTH,
		Clock::CHAR_HEIGHT);
      }

    if (hour != lastHour)
      {
	  lastHour = hour;

	  FAST short i = hour;

	  if (i > 9)
	    {
		if (i > 19)
		  {
		      i = 2;
		      hour -= 20;
		  }
		else
		  {
		      i = 1;
		      hour -= 10;
		  }
	    }
	  else
	    i = 0;

	  frect(location[0],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
	  shift(patterns[i],location[0],Clock::CHAR_WIDTH,
		Clock::CHAR_HEIGHT);

	  frect(location[1],Clock::CHAR_WIDTH,Clock::CHAR_HEIGHT,BLACK);
	  shift(patterns[hour],location[1],Clock::CHAR_WIDTH,
		Clock::CHAR_HEIGHT);
      }
}
