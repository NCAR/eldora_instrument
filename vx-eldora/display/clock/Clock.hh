/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Clock.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:53:12  thor
 * First offical ELDORA release!
 *
 * Revision 1.3  1991/05/06  15:18:50  thor
 * Changed from 8x16 cells to 12x24 cells. Moved size constants to public
 * view.
 *
 * Revision 1.2  1991/05/03  18:06:58  thor
 * Added commnets, inlined simpler methods.
 *
 * Revision 1.1  1991/05/03  15:19:37  thor
 * Initial revision
 *
 *
 *
 * description:
 *        The Clock class implements a simple digital clock. It
 * creates a single window for display and then changes the display
 * based on user commands - it does not keep time. There is a default
 * set of characters 0-9 & :. You may replace them but they must be
 * defined as follows:
 *    An 16x24 cell, a 1 bit means draw in the foreground color, 0 the
 * background color. Encoding is 14 bits per line, so you must pass a
 * pointer to a 24 short array. The top 4 bits of the short are unused.
 *
 * The memory organization is important and is currently very AGC
 * dependent. The displayed window takes up 136x32 pixels. The video
 * memory space for the characters takes 96x24 pixels. This is because
 * we leave a 4 pixel boarder and 4 pixels between cells. The data
 * space begins at 0,32 relative to the window's 0,0. 
 *
 * Speed of updating is accomplished by bliting the character required
 * into the proper cell.
 *
 * To maintain the displayability of the window, it always has the
 * highest window priority.
 *
 * Methods:
 * Clock(GraphicController *cntlr, int window, unsigned short x, unsigned
 * short y, unsigned short xoff, unsigned short yoff) - Create a clock
 * window at screen location x,y, using video memory at xoff,yoff. Use
 * window set window.
 *
 * void update(short hour, short minute, short second) - Update the
 * displayed time as required.
 *
 * void foreground(unsigned char fg) - 
 * void background(unsigned char fg) - Change the colors used. Only
 * takes effect when followed by calls to (re)set patterns.
 *
 * unsigned char foreground() - 
 * unsigned char background() - Return the current value.
 *
 * void display(void) - Show display window.
 *
 * void undisplay(void) - Hide window.
 *
 * void move(Point newPt) - Move window to new screen location.
 *
 * void setDefPatterns(void) - Move default characters into pattern
 * memory.
 *
 * void pattern(Point *ptr, unsigned char *pattern) - Put the pattern
 * indicated at the location specified.
 *
 */
#ifndef INCClockhh
#define INCClockhh
#pragma interface

#include "Window.hh"
#define OK_RPC
#include "DataBeam.hh"

class Clock : private Window {
  private:
    short lastHour;
    short lastMin;
    short lastSec;

    unsigned char fore;         // Colors.
    unsigned char back;

    Point patterns[11];
    Point location[8];

  public:

    enum sizes { CHAR_WIDTH = 12, CHAR_HEIGHT = 24, WIDTH = 136,
		 HEIGHT = 32 };
    
    Clock(GraphicController *cntlr, int window, unsigned short x, unsigned
	  short y, unsigned short xoff, unsigned short yoff);

    void update(short hour, short minute, short second);

    void update(DataBeam *beam)
      {
          update(beam->ray.hour,beam->ray.minute,beam->ray.second);
      }
    
    void foreground(unsigned char fg) { fore = fg; }

    unsigned char foreground(void) { return(fore); }

    void background(unsigned char bg) { back = bg; }

    unsigned char background(void) { return(back); }

    void display(void) { Window::display(); }

    void undisplay(void) { Window::undisplay(); }

    void move(Point newPt) { Window::move(newPt); }

    void setDefPatterns(void);

    void pattern(Point *ptr, unsigned short *pattern);

    ~Clock(void) { clear(); }
};

#endif /* INCClockhh */

