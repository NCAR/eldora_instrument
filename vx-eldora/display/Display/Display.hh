//
//	$Id$
//
//	Module:		 Display.hh
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        The class Display implements drawing of an RHI type display.
// It can currently draw up to 3 parameters. Methods are provided to
// draw a color table with associated units, title text, hash marks &
// text and most of the drawing & window operations available from the
// Window class. Methods are also available to display the window
// sets. These are the logical pairing of a data & table window. An
// embedded clock is provided via the Clock class member. This clock
// must be manually updated.
//
//
#ifndef INCDisplayhh
#define INCDisplayhh
#pragma interface

class Display;

#include "Window.hh"
#include "DataPoint.h"

// These defines are needed for rpc header.
#define OK_RPC
#define WRS_SIDE

#include <vxWorks.h>
#include <math.h>
#include <iostream.h>
#include "DisplayRpc.h"
#include "Header.hh"
#include "ParamNames.hh"
#include "ColorConverter.hh"
#include "Clock.hh"
#include "DataBeam.hh"
#include "point.h"
#include "mathVar.h"
#include "fastMath.h"

// This structure is used to hold the fixed point sines/cosines. The
// format is (fp value) << 16.
#ifndef _TRIGDATA
struct trigdata {
    int sin;
    int cos;
};

typedef struct trigdata TrigData;

#define _TRIGDATA
#endif

// Constants.

static const double BEAM_OFFSET = 20.0 * (M_PI / 180.0);

class Display {
  protected:
    Window Wdw[9];		// Maximum number of active windows.

    GraphicController *agc;	// TP-AGC object.
    ParamNames *namer;		// Text of parameter names.
    ColorConverter *converter;	// Counts to colors.
    Clock *clock;		// Running clock.
    
    int numOfParams;		// # of parameters in use.
    int numOfWdws;		// # of windows in use.
    int curZoom;		// Current zoom factor.
    int displayedSet;		// Who's on screen.
    int lastIndex;		// Last beam angle.
    int dispType;		// Who am I?

    TrigData *trigData;		// Angle data.
    
  public:
    enum sizes { FULL_WIDTH = 1024, FULL_HEIGHT = 1024, TBL_WIDTH = 256 };
    enum sets { A_SET, B_SET, C_SET };
    enum displays { RADIAL_FORE = 0x10, DUAL_FORE = 0x1000, VERT_FORE= 0x40,
                    HORIZ_FORE = 0x20, RADIAL_AFT = 0x80, DUAL_AFT = 0x2000,
                    VERT_AFT = 0x200, HORIZ_AFT = 0x100};

    Display(GraphicController *gbd);

    void setParmNames(ParamNames &p) { namer = &p; }

    void setColorConverter(ColorConverter &c) { converter = &c; }

    void setClock(Clock &c) { clock = &c; }

    int getType() { return(dispType); }
    
    virtual void reset(Header *hdr, DispCommand *cmd) = 0;
    
    virtual void drawBeam(DataBeam *beam) = 0;
    
    virtual void drawTable(int set, float max, float min, int param,
		   int tblsize = 31);
    
    virtual void drawTitle(int set, int radar) = 0;
    
    void setPriority(unsigned short priority);
    
    unsigned short getPriority(void);
    
    void center(Point center)
      {
          Wdw[0].center(center);
          Wdw[2].center(center);
          Wdw[4].center(center);
      }
    
    void pan(Point panOrigin)
      {
          Wdw[0].pan(panOrigin);
          Wdw[2].pan(panOrigin);
          Wdw[4].pan(panOrigin);
      }
    
    void home(void)
      {
          Wdw[0].home();
          Wdw[2].home();
          Wdw[4].home();
      }
    
    void setZoom(unsigned short zoom)
      {
          Wdw[0].setZoom(zoom);
          Wdw[2].setZoom(zoom);
          Wdw[4].setZoom(zoom);
      }
    
    unsigned short getZoom(void);
    
    void line(int wdw, Point start, Point end, unsigned char color);
    
    void arc(int wdw, Point center, Point left, Point right, int rad, 
	     unsigned char color);
    void circle(int wdw, Point center, int rad, unsigned char color);
    void fcircle(int wdw, Point center, int rad,
		 unsigned char color);
    
    void rect(int wdw, Point corner, int width, int height, 
	      unsigned char color);
    void frect(int wdw, Point left, Point right,
	       unsigned char color);
    void frect(int wdw, Point corner, int width, int height, 
	       unsigned char color);
    
    void tri(int wdw, Point one, Point two, Point three,
	     unsigned char color);
    void ftri(int wdw, Point one, Point two, Point three,
	      unsigned char color);
    
    void point(int wdw, Point pt, unsigned char color);
    
    void clear(int wdw);
    
    void setTextScale(int wdw, int x, int y);
    
    void setFont(const char *font);
    
    void setTextBackGround(int wdw, int color);
    
    void horText(int wdw, Point start, const char *text,
		 unsigned char color);
    void horCenteredText(int wdw, Point start, const char *text, 
			 unsigned char color);
    
    void vertText(int wdw, Point start, const char *text,
		  unsigned char color);
    void vertCenteredText(int wdw, Point start, const char *text, 
			  unsigned char color);
    
    virtual void displaySet(int set) = 0;
    
    virtual void switchSets(void);
    
    void nextZoom(Point cursor);
    
    virtual void undisplay(void);

    void updateClock(DataBeam *beam) { clock->update(beam); }

    void moveClock(Point p) { clock->move(p); }

    void showClock() { clock->display(); }

    void hideClock() { clock->undisplay(); }

    virtual int wdwOk(int wdw);		// Is wdw active?
    
    virtual ~Display(void);

};

#endif // INCDisplayhh
