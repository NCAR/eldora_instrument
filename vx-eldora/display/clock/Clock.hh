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
 * The memory organization is important and is currently very AGV
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
 * void Update(short hour, short minute, short second) - Update the
 * displayed time as required.
 *
 * void Foreground(unsigned char fg) - 
 * void Background(unsigned char fg) - Change the colors used. Only
 * takes effect when followed by calls to (re)set patterns.
 *
 * unsigned char Foreground() - 
 * unsigned char Background() - Return the current value.
 *
 * void Display(void) - Show display window.
 *
 * void Undisplay(void) - Hide window.
 *
 * void Move(Point newPt) - Move window to new screen location.
 *
 * void SetDefPatterns(void) - Move default characters into pattern
 * memory.
 *
 * void Pattern(Point *ptr, unsigned char *pattern) - Put the pattern
 * indicated at the location specified.
 *
 */
#ifndef INCClockhh
#define INCClockhh

#include "Window.hh"

static const int CLKWIDTH = 12;
static const int CLKHEIGHT = 24;

class Clock : private Window {
  private:
    short lastHour;
    short lastMin;
    short lastSec;

    unsigned char foreground;
    unsigned char background;

    Point patterns[11];
    Point location[8];

  public:
    Clock(GraphicController *cntlr, int window, unsigned short x, unsigned
	  short y, unsigned short xoff, unsigned short yoff);

    void Update(short hour, short minute, short second);

    void Foreground(unsigned char fg) { foreground = fg; }

    unsigned char Foreground(void) { return(foreground); }

    void Background(unsigned char bg) { background = bg; }

    unsigned char Background(void) { return(background); }

    void Display(void) { display(); }

    void Undisplay(void) { undisplay(); }

    void Move(Point newPt) { move(newPt); }

    void SetDefPatterns(void);

    void Pattern(Point *ptr, unsigned short *pattern);

    ~Clock(void) { clear(); }
};

#endif /* INCClockhh */

