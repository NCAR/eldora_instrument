/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Date.hh
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
#ifndef INCDatehh
#define INCDatehh

#include "Window.hh"

static const int DATE_WIDTH = 136;
static const int DATE_HEIGHT = 32;


class Date : private Window {
  private:
    short lastMonth;
    short lastDay;
    short lastYear;

    void Draw(void);

  public:
    Date(GraphicController *cntlr, int window, unsigned short x, unsigned
	 short y, unsigned short xoff, unsigned short yoff);

    void Set(short day, short month, short year);

    void NextDay(void);

    void Display(void) { display(); }

    void Undisplay() { undisplay(); }
};

#endif // INCDatehh
