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
 * Revision 1.2  1991/05/03  18:05:57  thor
 * Added comments, fixed incorrect call to copyBlock.
 *
 * Revision 1.1  1991/05/03  15:19:17  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This contains the methods for the Clcok class.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Clock.hh"
#include "ClockP.hh"

Clock::Clock(GraphicController *cntlr, int window, unsigned short x, unsigned
	     short y, unsigned short xoff, unsigned short yoff) : 
	     Window(cntlr, window, x, y, 136, 32, xoff, yoff)
{
    foreground = WHITE1;	// Default colors are white on black.
    background = BLACK;

    lastHour = -1;		// Force redraw on first update.
    lastMin = -1;
    lastSec = -1;

    FAST Point *ptr = location;

    FAST int j = 8;
    FAST unsigned short k = 4 + CLKWIDTH;
    FAST unsigned short lx = 4;	// 4 pixel wide boarders.
    FAST unsigned short ly = 4;

    for (FAST int i = 0; i < j; i++, ptr++, lx += k)
      {
	  ptr->x = lx;
	  ptr->y = ly;
      }

    j = 11;

    lx = 0;
    ly = CLKHEIGHT + 8;		// Start data storage directly below
				// displayed area.

    ptr = patterns;

    for (i = 0; i < j; i++, ptr++, lx += CLKWIDTH)
      {
	  ptr->x = lx;
	  ptr->y = ly;
      }

    SetDefPatterns();

    setPriority(MAXWDWPRI);
}

void Clock::SetDefPatterns(void)
{
    FAST Point *ptr = patterns;

    Pattern(ptr++,ZeroMap);
    Pattern(ptr++,OneMap);
    Pattern(ptr++,TwoMap);
    Pattern(ptr++,ThreeMap);
    Pattern(ptr++,FourMap);
    Pattern(ptr++,FiveMap);
    Pattern(ptr++,SixMap);
    Pattern(ptr++,SevenMap);
    Pattern(ptr++,EightMap);
    Pattern(ptr++,NineMap);
    Pattern(ptr,ColonMap);

    // Clear displayed area and draw colons.
    Point a;
    a.x = 0;
    a.y = 0;
    frect(a,CLKWIDTH,CLKHEIGHT,BLACK);
    shift(patterns[10],location[2],CLKWIDTH,CLKHEIGHT);
    shift(patterns[10],location[5],CLKWIDTH,CLKHEIGHT);
}

void Clock::Pattern(FAST Point *ptr, FAST unsigned short *pattern)
{
    Point pt = *ptr;		// Where is data space to draw character.

    FAST int j = CLKHEIGHT;
    FAST int l = CLKWIDTH;

    // Loop drawing each line.
    for (FAST int i = 0; i < j; i++, pattern++)
      {
	  FAST unsigned short p = *pattern;

	  p <<= 4;		// Get rid of unused top end nibble.

	  for (FAST int k = 0; k < l; k++)
	    {
		if ((p & 0x8000))	// Set bit means draw foreground color.
		  point(pt,foreground);
		else
		  point(pt,background);

		p <<= 1;

		pt.x += 1;
	    }
	  pt.x = ptr->x;	// Get ready for start of next line.
	  pt.y += 1;
      }
}

void Clock::Update(FAST short hour, FAST short minute, FAST short second)
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
	  
	  frect(location[6],CLKWIDTH,CLKHEIGHT,BLACK);
	  shift(patterns[i],location[6],CLKWIDTH,CLKHEIGHT);
	  
	  frect(location[7],CLKWIDTH,CLKHEIGHT,BLACK);
	  shift(patterns[second],location[7],CLKWIDTH,CLKHEIGHT);
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

	  frect(location[3],CLKWIDTH,CLKHEIGHT,BLACK);
	  shift(patterns[i],location[3],CLKWIDTH,CLKHEIGHT);

	  frect(location[4],CLKWIDTH,CLKHEIGHT,BLACK);
	  shift(patterns[minute],location[4],CLKWIDTH,CLKHEIGHT);
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

	  frect(location[0],CLKWIDTH,CLKHEIGHT,BLACK);
	  shift(patterns[i],location[0],CLKWIDTH,CLKHEIGHT);

	  frect(location[1],CLKWIDTH,CLKHEIGHT,BLACK);
	  shift(patterns[hour],location[1],CLKWIDTH,CLKHEIGHT);
      }
}
