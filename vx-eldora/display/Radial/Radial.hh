/*
 *	$Id$
 *
 *	Module:		 Radial.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.6  1991/10/23  14:42:43  thor
 * Added some needed includes.
 *
 * Revision 1.5  1991/10/09  18:03:28  thor
 * Added forward class definition.
 *
 * Revision 1.4  1991/06/13  17:50:29  thor
 * Add references to rpc headers. Added SetBounds method.
 * Changes drawTitle method to use radar type instead of parameter.
 *
 * Revision 1.3  1991/05/06  15:11:45  thor
 * Added embedded Clock class for time display.
 *
 * Revision 1.2  1991/04/29  18:46:53  thor
 * Changed calling of drawing routines to use new RadialData format.
 *
 * Revision 1.1  1991/04/08  20:29:04  thor
 * Initial revision
 *
 *
 * description:
 *        The class Radial implements drawing of an RHI type display.
 * It can currently draw up to 3 parameters. Methods are provided to
 * draw a color table with associated units, title text, hash marks &
 * text and most of the drawing & window operations available from the
 * Window class. Methods are also available to display the window
 * sets. These are the logical pairing of a data & table window. An
 * embedded clock is provided via the Clock class member. This clock
 * must be manually updated.
 *
 */
#ifndef INCRadialhh
#define INCRadialhh

class Radial;

#include "Clock.hh"
#include "Window.hh"
#include "DataPoint.h"

// These defines are needed for rpc header.
#define OK_RPC
#define WRS_SIDE

extern "C" {
#include "vxWorks.h"
#include "math.h"
#include "DisplayRpc.h"
};

#include "ParamNames.h"
#include "point.h"


// This structure is used to hold the fixed point sines/cosines. The
// format is (fp value) << 16.
struct trigdata {
    int sin;
    int cos;
};

typedef struct trigdata TrigData;

// Constants.

// Window sizes.
static const int PLOT_WIDTH  = 1024;
static const int PLOT_HEIGHT = 1024;
static const int TBL_WIDTH   = 256;
static const int TBL_HEIGHT  = 1024;

static float BEAM_OFFSET = 20.0 * (M_PI / 180.0);

class Radial {
  private:
    Window Wdw[6];

    unsigned char *videoMemory[3];
    unsigned char *videoBase[3];

    int numOfParams;
    int curZoom;
    int displayedSet;
    int lastIndex;
    int radius;
    int firstGate;

    float maxDistance;

    TrigData *trigData;

    Clock clk;

    void Draw1(RadialData &data);
    void Draw2(RadialData &data);
    void Draw3(RadialData &data);

  public:
    Radial(GraphicController *gbd, int rad, int nparams,
           unsigned short xoff, unsigned short yoff);

    void drawBeam(FAST RadialData &data)
      {
	  FAST int np = numOfParams;

	  if (np == 1)
	    Draw1(data);
	  else if (np == 2)
	    Draw2(data);
	  else
	    Draw3(data);
      }
    
    void drawTable(int set, float max, float min, int param,
		   int tblsize = 31);
    
    void drawTitle(int set, int radar);
    
    void SetBounds(float max, float first);

    void expose(int wdw);
    
    void hide(int wdw);
    
    void setPriority(unsigned short priority)
      {
          Wdw[0].setPriority(priority);
          Wdw[1].setPriority(priority);
          Wdw[2].setPriority(priority);
          Wdw[3].setPriority(priority);
          Wdw[4].setPriority(priority);
          Wdw[5].setPriority(priority);
      }
    
    unsigned short getPriority(void);
    
    int move(Point newOrigin);
    
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
    
    void display(int wdw);

    void undisplay(int wdw);
    
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
    
    void setFont(char *font);
    
    void setTextBackGround(int wdw, int color);
    
    void horText(int wdw, Point start, char *text,
		 unsigned char color);
    void horCenteredText(int wdw, Point start, char *text, 
			 unsigned char color);
    
    void vertText(int wdw, Point start, char *text,
		  unsigned char color);
    void vertCenteredText(int wdw, Point start, char *text, 
			  unsigned char color);
    
    void displaySet(int set)
      {
          if (set == A_SET)
            {
                Wdw[0].display();
                Wdw[1].display();
                displayedSet = A_SET;
            }
          else if (set == B_SET)
            {
                Wdw[2].display();
                Wdw[3].display();
                displayedSet = B_SET;
            }
          else
            {
                Wdw[4].display();
                Wdw[5].display();
                displayedSet = C_SET;
            }
      }
    
    void switchSets(void)
      {
	  FAST int nparams = numOfParams;
	  FAST int dset = displayedSet;

          if (dset == A_SET)
            {
		if (nparams > 1)
		  {
		      dset = B_SET;
		      
		      Wdw[0].undisplay();
		      Wdw[1].undisplay();
		      
		      Wdw[2].display();
		      Wdw[3].display();
		  }
	    }
          else if (dset == B_SET)
            {
                Wdw[2].undisplay();
                Wdw[3].undisplay();

		if (nparams == 2)
		  {
		      dset = A_SET;

		      Wdw[0].display();
		      Wdw[1].display();
		  }
		else
		  {
		      dset = C_SET;

		      Wdw[4].display();
		      Wdw[5].display();
		  }
            }
          else
            {
                dset = A_SET;
                Wdw[4].undisplay();
                Wdw[5].undisplay();

                Wdw[0].display();
                Wdw[1].display();
            }
	  displayedSet = dset;
      }
    
    void nextZoom(Point cursor);
    
    void centerAll(Point newCenter)
      {
          Wdw[0].center(newCenter);
          Wdw[2].center(newCenter);
          Wdw[4].center(newCenter);
      }
    
    void homeAll(void)
      {
          Wdw[0].home();
          Wdw[2].home();
          Wdw[4].home();
      }

    void UpdateClock(FAST short h, FAST short m, FAST short s)
      {
	  clk.Update(h,m,s);
      }

    ~Radial(void);

};

#endif INCRadialhh
