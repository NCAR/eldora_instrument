/*
 *	$Id$
 *
 *	Module:		 Window.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.1  1993/07/29  17:55:14  thor
 * Upgraded to VxWorks 5.1.
 *
 * Revision 2.0  1992/11/03  12:51:14  thor
 * First offical ELDORA release!
 *
 * Revision 1.4  1992/03/16  21:29:45  thor
 * Added aPan method to allow raw panning.
 *
 * Revision 1.3  1991/02/27  16:56:44  thor
 * Added shift, copyBlock and rootXY methods.
 *
 * Revision 1.2  91/01/02  08:52:23  thor
 * Made changes for VxWorks Version 5.0.
 * 
 * Revision 1.1  90/12/04  10:23:17  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        The Window class allows a logical window to be constructed
 * that maps a physical area of memory in a logical construct that
 * defines its screen location, size, zoom factor, etc.
 *
 */
#ifndef INCWindowhh
#define INCWindowhh

#include "GraphicController.hh"

#include "vxWorks.h"

#include "point.h"

#ifdef WINDOW_CLASS
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#endif

/* Maximum window priority */
static const unsigned short MAXWDWPRI = 7;

/* Maximum displayable screen size. */
static const unsigned short MAX_X_WIDTH = 1280;
static const unsigned short MAX_Y_HEIGHT = 1024;

/* "Color" that says don't touch background text pixels */
static const int DONT_TOUCH = -1;

class Window {
  private:
    WindowControlRegs controlSetting;

    void *vmeLocation;

    GraphicController *gbd;

    int window;
    int displayState;
    int currentMask;

    int textScaleX;
    int textScaleY;
    int backGroundColor;
    char *fontList;

  protected:
    Point upperLeft;
    Point bottomRight;

  public:
    Window(GraphicController *cntlr, int window, unsigned short x, unsigned 
	   short y, unsigned short xwidth, unsigned short yheight,
	   unsigned short xoff, unsigned short yoff);

    Window(void) {}

    void init(GraphicController *cntlr, int wdw, unsigned short x, 
	       unsigned short y, unsigned short xwidth, unsigned short yheight,
	       unsigned short xoff, unsigned short yoff);

    void expose(void) {
	setPriority(MAXWDWPRI);
    }

    void hide(void) {
	setPriority(0);
    }

    void setPriority(unsigned short priority);

    unsigned short getPriority(void) {
	return(controlSetting.viewPriority);
    }

    int resize(unsigned short newWidth, unsigned short newHeight);

    int move(Point newOrigin);

    int center(Point center);

    int pan(Point panOrigin);

    void aPan(Point newOrigin);

    void home(void);

    void setZoom(unsigned short zoom);

    unsigned short getZoom(void) {
	return(controlSetting.zoom & 0xf);
    }

    int setMask(int mask) {
	int oldMask = currentMask;
	currentMask = mask;
	return(oldMask);
    }

    int getMask(void) {
	return(currentMask);
    }

    void display(void);
    void undisplay(void);

    void line(Point start, Point end, unsigned char color);

    void arc(Point center, Point left, Point right, int rad, 
	     unsigned char color);
    void circle(Point center, int rad, unsigned char color);
    void fcircle(Point center, int rad, unsigned char color);

    void rect(Point corner, int width, int height, unsigned char color);
    void frect(Point left, Point right, unsigned char color);
    void frect(Point corner, int width, int height, unsigned char color);

    void tri(Point one, Point two, Point three, unsigned char color);
    void ftri(Point one, Point two, Point three, unsigned char color);

    void point(Point pt, unsigned char color);

    void text(Point start, const char *text, unsigned char color,
	      float angle = 0.0);

    void clear(void);

    void setTextScale(int x, int y);

    void setFont(char *font);

    void setTextBackGround(int color);

    void horText(Point start, const char *text, unsigned char color);
    void horCenteredText(Point start, const char *text, unsigned char color);

    void vertText(Point start, const char *text, unsigned char color);
    void vertCenteredText(Point start, const char *text, unsigned char color);

    void shift(Point src, Point dst, int width, int height);

    void copyBlock(Point src, Point dst, int width, int height);

    Point rootXY(Point pt);
};    

#endif INCWindowhh
