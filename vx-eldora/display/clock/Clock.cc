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
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Clock.hh"
#include "ClockP.hh"

Clock::Clock(GraphicController *cntlr, int window, unsigned short x, unsigned
	     short y, unsigned short xoff, unsigned short yoff) : 
	     Window(cntlr, window, x, y, 100, 24, xoff, yoff)
{
    foreground = WHITE1;
    background = BLACK;

    lastHour = -1;
    lastMin = -1;
    lastSec = -1;

    FAST Point *ptr = location;

    FAST int j = 8;
    FAST unsigned short lx = 4;
    FAST unsigned short ly = 4;

    for (FAST int i = 0; i < j; i++, ptr++, lx += 12)
      {
	  ptr->x = lx;
	  ptr->y = ly;
      }

    j = 11;

    lx = 0;
    ly = 24;

    ptr = patterns;

    for (i = 0; i < j; i++, ptr++, lx += CLKWIDTH)
      {
	  ptr->x = lx;
	  ptr->y = ly;
      }

    SetPatterns();

    setPriority(MAXWDWPRI);
}

void Clock::SetPatterns(void)
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

    frect(location[2],CLKWIDTH,CLKHEIGHT,BLACK);
    frect(location[5],CLKWIDTH,CLKHEIGHT,BLACK);
    copyBlock(patterns[10],location[2],CLKWIDTH,CLKHEIGHT);
    copyBlock(patterns[10],location[5],CLKWIDTH,CLKHEIGHT);
}

void Clock::Pattern(FAST Point *ptr, FAST unsigned char *pattern)
{
    Point pt = *ptr;

    FAST int j = CLKHEIGHT;

    for (FAST int i = 0; i < j; i++, pattern++)
      {
	  FAST int l = CLKWIDTH;

	  FAST unsigned char p = *pattern;

	  for (FAST int k = 0; k < l; k++)
	    {
		if ((p & 0x80))
		  point(pt,foreground);
		else
		  point(pt,background);

		p <<= 1;

		pt.x += 1;
	    }
	  pt.x = ptr->x;
	  pt.y += 1;
      }
}

void Clock::Foreground(unsigned char fg)
{
    foreground = fg;

    SetPatterns();
}

void Clock::Background(unsigned char bg)
{
    background = bg;

    SetPatterns();
}

void Clock::Display(void)
{
    display();
}

void Clock::Undisplay(void)
{
    undisplay();
}

void Clock::Move(Point newPt)
{
    move(newPt);
}

void Clock::Update(FAST short hour, FAST short minute, FAST short second)
{
    if (second != lastSec)
      {
	  lastSec = second;

	  FAST short i = second;

	  if (i > 9)
	    {
		i /= 10;

		second -= (i * 10);
	    }
	  else
	    i = 0;
	  
	  frect(location[6],CLKWIDTH,CLKHEIGHT,BLACK);
	  copyBlock(patterns[i],location[6],CLKWIDTH,CLKHEIGHT);
	  
	  frect(location[7],CLKWIDTH,CLKHEIGHT,BLACK);
	  copyBlock(patterns[second],location[7],CLKWIDTH,CLKHEIGHT);
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
	  copyBlock(patterns[i],location[3],CLKWIDTH,CLKHEIGHT);

	  frect(location[4],CLKWIDTH,CLKHEIGHT,BLACK);
	  copyBlock(patterns[minute],location[4],CLKWIDTH,CLKHEIGHT);
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
	  copyBlock(patterns[i],location[0],CLKWIDTH,CLKHEIGHT);

	  frect(location[1],CLKWIDTH,CLKHEIGHT,BLACK);
	  copyBlock(patterns[hour],location[1],CLKWIDTH,CLKHEIGHT);
      }
}
