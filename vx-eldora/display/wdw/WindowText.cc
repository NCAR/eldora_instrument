/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 WindowText.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  91/01/02  08:51:05  thor
 * Made changes for VxWorks Version 5.0.
 * 
 * Revision 1.1  90/12/04  10:22:05  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        
 */
#include "Window.hh"

extern "C" {
#include "strLib.h"
};

void Window::setTextScale(FAST int x, FAST int y)
{
    textScaleX = x;
    textScaleY = y;
}

void Window::setFont(char *font)
{
    fontList = font;
}

void Window::setTextBackGround(int color)
{
    backGroundColor = color;
}

void Window::horText(Point start, FAST char *text, FAST unsigned char color)
{
    FAST unsigned short x = start.x + upperLeft.x;
    FAST unsigned short y = start.y + upperLeft.y;

    FAST unsigned short xsize = textScaleX;
    FAST unsigned short ysize = textScaleY;

    x -= xsize ? xsize * 7 : 4;
    y -= ysize ? ysize * 4 : 2;

    FAST unsigned short yend = y + (ysize ? 14 * ysize : 7);

    FAST unsigned char *vme = vmeLocation;

    for (FAST int i = 0; *text; i++, text++)
      {
	  FAST char *ptr = fontList + (*text << 4);

	  for (FAST int j = y; j < yend; j++)
	    {
		FAST int l = 128;
		for (FAST int k = x + (i << 3) * (xsize ? xsize : 0.5); l; k++)
		  {
		      if (!(l & *ptr))
			{
			    if (backGroundColor != DONT_TOUCH)
			      *(vme + k + (j * 4096)) = backGroundColor 
				& 0xff;
			}
		      else
			*(vme + k + (j * 4096)) = color;
		      
		      if (!xsize)
			l >>= 2;
		      else if (k % xsize == xsize - 1)
			l >>= 1;
		  }

		if (!ysize)
		  ptr += 2;
		else if (j % ysize == ysize - 1)
		  ptr++;
	    }
      }
}

void Window::horCenteredText(Point start, char *text, unsigned char color)
{
    FAST int xsize = textScaleX;

    start.x -= (strlen(text) >> 1) * (xsize ? xsize * 8 : 4);

    horText(start,text,color);
}

void Window::vertText(Point start, FAST char *text, FAST unsigned char color)
{
    FAST int x = start.x + upperLeft.x;
    FAST int y = start.y + upperLeft.y;

    FAST int xsize = textScaleX;
    FAST int ysize = textScaleY;

    x -= xsize ? 4 * xsize : 2;
    y -= ysize ? 7 * ysize : 4;

    FAST int yend = y + (ysize ? 14 * ysize : 7);
    FAST unsigned char *vme = vmeLocation;

    for (FAST int i = 0; *text; i++, text++, yend += ysize ? 14 * ysize : 7)
      {
	  if (*text <= 0x1f)
	    continue;

	  FAST char *ptr = fontList + (*text << 4);

	  for (FAST int j = y + (i * (ysize ? 14 * ysize : 7)); j < yend; j++)
	    {
		FAST int l = 128;

		for (FAST int k = x; l; k++)
		  {
		      if (!(l & *ptr))
			{
			    if (backGroundColor != DONT_TOUCH)
			      *(vme + k + (j * 4096)) = backGroundColor 
				& 0xff;
			}
		      else
			*(vme + k + (j * 4096)) = color;

		      if (!xsize)
			l >>= 2;
		      else if (k % xsize == xsize - 1)
			l >>= 1;
		  }
		if (!ysize)
		  ptr += 2;
		else if (j % ysize == ysize - 1)
		  ptr++;
		}
      }
}

void Window::vertCenteredText(Point start, char *text, unsigned char color)
{
    FAST int ysize = textScaleY;

    start.y -= (strlen(text) >> 1) * (ysize ? 14 * ysize : 7);

    vertText(start,text,color);
}
