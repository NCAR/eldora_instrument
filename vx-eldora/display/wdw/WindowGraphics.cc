/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 WindowGraphics.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/02/27  16:58:31  thor
 * Added shift, copyBlock and rootXY methods.
 *
 * Revision 1.1  90/12/04  10:21:51  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        These are the methods used for graphic specific functions of
 * the Window class. To simplify the use of coordinates from the
 * user's viewpoint, all windows are addressed as if their upper left
 * corner is at 0,0. The actual offset in memory is then added by the
 * method to get the correct x/y location. 
 *
 */
#include "Window.hh"

void Window::line(Point start, Point end, FAST unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    start.x += x;
    start.y += y;

    end.x += x;
    end.y += y;

    gbd->drawLine(start,end,color);
}

void Window::arc(Point center, Point left, Point right, FAST int rad,
		 FAST unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    center.x += x;
    center.y += y;
    left.x += x;
    left.y += y;
    right.x += x;
    right.y += y;

    gbd->drawArc(center,left,right,rad,color);
}
		 
void Window::circle(Point center, FAST int rad, FAST unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    center.x += x;
    center.y += y;

    gbd->drawCircle(center,rad,color);
}

void Window::fcircle(Point center, FAST int rad, FAST unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    center.x += x;
    center.y += y;

    gbd->drawFilledCircle(center,rad,color);
}

void Window::rect(Point corner, int width, int height, unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    corner.x += x;
    corner.y += y;

    Point right;
    right.x = corner.x + width;
    right.y = corner.y + height;

    gbd->drawRect(corner,right,color);

}

void Window::frect(Point left, Point right, unsigned char color)
{
    frect(left,right.x - left.x,right.y - left.y, color);
}

void Window::frect(Point corner, int width, int height, unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    corner.x += x;
    corner.y += y;

    Point right;
    right.x = corner.x + width;
    right.y = corner.y + height;

    gbd->drawFilledRect(corner,right,color);

}

void Window::tri(Point one, Point two, Point three, unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    one.x += x;
    one.y += y;

    two.x += x;
    two.y += y;

    three.x += x;
    three.y += y;

    gbd->drawTri(one,two,three,color);
}

void Window::ftri(Point one, Point two, Point three, unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    one.x += x;
    one.y += y;

    two.x += x;
    two.y += y;

    three.x += x;
    three.y += y;

    gbd->drawFilledTri(one,two,three,color);
}

void Window::point(Point pt, unsigned char color)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    pt.x += x;
    pt.y += y;

    gbd->drawPoint(pt,color);
}

void Window::text(Point start, char *text, unsigned char color,
		  float angle = 0.0)
{
}

void Window::clear(void)
{
    frect(upperLeft,bottomRight,BLACK);
}

void Window::shift(Point src, Point dst, FAST int width, FAST int height)
{
    FAST unsigned short x = upperLeft.x;
    FAST unsigned short y = upperLeft.y;

    src.x += x;
    src.y += y;
    dst.x += x;
    dst.y += y;

    gbd->shift(src,dst,width,height);
}

void Window::copyBlock(Point src, Point dst, FAST int width, FAST int height)
{
    gbd->shift(src,dst,width,height); // Uses raw x,y values - good
				      // for moving blocks between windows.
}

Point Window::rootXY(Point pt)
{
    pt.x += upperLeft.x;
    pt.y += upperLeft.y;

    return(pt);
}
