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
 *
 *
 * description:
 *        
 */
#ifndef INCClockhh
#define INCClockhh

#include "Window.hh"

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

    void Foreground(unsigned char fg);

    unsigned char Foreground(void) { return(foreground); }

    void Background(unsigned char bg);

    unsigned char Background(void) { return(background); }

    void Display(void);

    void Undisplay(void);

    void Move(Point newPt);

    void SetPatterns(void);

    void Pattern(FAST Point *ptr, FAST unsigned char *pattern);
};

#endif /* INCClockhh */

