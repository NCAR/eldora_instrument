/*
 *	$Id$
 *
 *	Module:		 GraphicController.hh
 *	Original Author: Richard E. H. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.2  1993/07/29  17:55:14  thor
 * Upgraded to VxWorks 5.1.
 *
 * Revision 2.1  1993/07/01  16:20:03  thor
 * Brought code up to latest ANSI draft spec.
 *
 * Revision 2.0  1992/11/03  12:51:14  thor
 * First offical ELDORA release!
 *
 * Revision 1.5  1991/10/21  17:32:14  thor
 * Added clear method.
 *
 * Revision 1.4  1991/03/01  21:07:40  thor
 * Cleaned up unused consts. Moved colors to WdwGbls.h.
 *
 * Revision 1.3  1991/02/28  14:44:02  thor
 * Updated default colors.
 *
 * Revision 1.2  91/01/02  08:52:11  thor
 * Made changes for VxWorks Version 5.0.
 * 
 * Revision 1.1  90/12/04  10:22:55  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This header file contains the description of the
 * GraphicController class. Currently it is built around the Tadpole
 * TP-AGCV  graphics board. 
 *
 */
#ifndef INCGraphicControllerhh
#define INCGraphicControllerhh

#include "vxWorks.h"
#include "semLib.h"

#include "point.h"
#include "qpdm.h"
#include "WdwGbls.h"

/* Private header. */
#include "GraphicControllerP.hh"

/* Maximum priority for a window and the legal zoom factors. */
static const unsigned short MAX_WDW_PRI = 7;
static const unsigned short ZOOM1 = 0;
static const unsigned short ZOOM2 = 0x5;
static const unsigned short ZOOM4 = 0xa;
static const unsigned short ZOOM8 = 0xf;

/* The lower four bits of a vector for each functional unit. */
static const unsigned char FIFO_VECTOR = 0x2;
static const unsigned char QPDM_VECTOR = 0xe;
static const unsigned char MCU_VECTOR = 0x8;
static const unsigned char FRAME_VECTOR = 0xb;

/* The AGCV window register layout */
struct WindowControlRegs {
    unsigned short startx;
    unsigned short starty;
    unsigned short width;
    unsigned short height;
    unsigned short xoffset;
    unsigned short yoffset;
    unsigned short viewPriority;
    unsigned short zoom;
};

class GraphicController {
  private:
    WindowRegisters windows[4]; /* Windows 0-3 are 8 bits deep. */

    SEM_ID accessSem;
    
    void *baseAddr;

    unsigned short *gcr;
    unsigned char *fifoFlags;
    unsigned char *maskReg;
    unsigned char *vectorReg;

    void *qpdmFifo;

    long oneBitWindows[8];		/* Base for work on 1-bit windows. */

    unsigned short lastQpdmOp;
    unsigned char lastColor;
    

  public:
    GraphicController(void *addr, unsigned short screenWidth, unsigned short
		      screenHeight, unsigned short memWidth, unsigned char
		      vector);

    void clear(void);

    /* This method is in GraphicWindow.cc */
    int setGraphicOp(int op);

    /* These methods are window specific - see GraphicWindow.cc */
    void priority(int window, unsigned short priority);

    void zoom(int window, unsigned short zoom);

    void display(int window, WindowControlRegs *settings);

    void undisplay(int window);

    void pan(int window, long newLocation);

    void screenMove(int window, long newLocation);

    void screenResize(int window, long newLocation);

    int setMask(int mask);

    int getMask(void) {
	return(*maskReg & 0xff);
    }

    /* See GraphicColors.cc */
    void setColorMap(long *colors, int mapSize);

    void setOverlayColorMap(long color);

    void defaultColorMap(void);

    /* See GraphicShift.cc */
    void shift(Point src, Point dst, int width, int height);

    /* See GraphicLines.cc */
    void drawLine(Point start, Point end, unsigned char color);

    /* See GraphicRects.cc */
    void drawRect(Point left, Point right, unsigned char color);
    void drawFilledRect(Point left, Point right, unsigned char color);

    /* See GraphicTri.cc */
    void drawTri(Point one, Point two, Point three, unsigned char color);
    void drawFilledTri(Point one, Point two, Point three, unsigned char color);

    /* See GraphicCircles.cc */
    void drawArc(Point center, Point left, Point right, int radius, 
		 unsigned char color);
    void drawCircle(Point center, int radius, unsigned char color);
    void drawFilledCircle(Point center, int radius, unsigned char color);

    /* These are very AGCV specific! */
    void setQpdnOp(unsigned short op) {
	lastQpdmOp = op;
    }

    void drawPoint(FAST Point pt, unsigned char color) {

	FAST unsigned char *ptr = (pt.y * 4096) + pt.x +
	  (unsigned char *)baseAddr;
	
	*ptr = color;
    }

    void *videoBufferAddr(void) {
	return(baseAddr);
    }

    void qpdmCmd(long *cmd, int number); 
};

#endif INCGraphicControllerhh
