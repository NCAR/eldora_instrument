/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Dual.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/08/31  16:39:41  thor
 * The first really working version.
 *
 * Revision 1.1  1992/11/06  20:20:24  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCDualhh
#define INCDualhh

class Dual;

#include "Clock.hh"
#include "Window.hh"
#include "DataPoint.h"

// These defines are needed for rpc header.
#define OK_RPC
#define WRS_SIDE


#include "vxWorks.h"
#include "math.h"
#include "DisplayRpc.h"

#include "mathVar.h"
#include "ParamNames.h"
#include "CellSpacing.h"
#include "point.h"
#include "fastMath.h"

// This structure is used to hold the fixed point sines/cosines. The
// format is (fp value) << 16.
#ifndef _TRIGDATA
struct trigdata {
    int sin;
    int cos;
};

typedef struct trigdata TrigData;
#endif
// Constants.

// Window sizes.
static const int DUAL_PLOT_WIDTH  = 1024;
static const int DUAL_PLOT_HEIGHT = 512;
static const int DUAL_PLOT_MEM    = DUAL_PLOT_WIDTH;
static const int DUAL_TBL_WIDTH   = 256;
static const int DUAL_TBL_HALF    = 512;
static const int DUAL_TBL_HEIGHT  = 1024;

static double BEAMWIDTH = M_PI / 180.0;

class Dual {
  private:
    Window TopData;
    Window BottomData;
    Window Tbl;

    unsigned char *videoMemory[2];

    int curZoom;
    int lastIndex;
    int radius;
    int firstGate;

    float maxAlt;
    float minAlt;
    float pixelsPerMeter;

    int direction;

    TrigData *trigData;
    
    Clock clk;

    int clip(FAST int x , FAST int y)
    {
	if ((x < 0) || (x >= DUAL_PLOT_WIDTH) || (y < 0) ||
	    (y >= DUAL_PLOT_HEIGHT))
	  {
	      return(0);
	  }
	return(1);
    }

    void plot(FAST unsigned char *v, FAST int x, FAST int y,
	     FAST unsigned char c)
    {
	v += x + (y * 4096);

	*v = c;
    }

  public:
    Dual(GraphicController *gbd, int rad, unsigned short xoff,
	 unsigned short yoff);

    void DrawBeam(FAST DualData &data);
    
    void DrawTable(int set, float max, float min, int param,
                   int tblsize = 31);
    
    void DrawTitle();
    
    void SetBounds(float max, float min, float first);

    void Center(Point center)
      {
          TopData.center(center);
          BottomData.center(center);
      }
    
    void Pan(Point panOrigin)
      {
          TopData.pan(panOrigin);
          BottomData.pan(panOrigin);
      }
    
    void Home(void)
      {
          TopData.home();
          BottomData.home();
      }
    
    void SetZoom(unsigned short zoom)
      {
          TopData.setZoom(zoom);
          BottomData.setZoom(zoom);
      }
    
    unsigned short GetZoom(void);

    void Display(void);

    void Undisplay(void);
    
    void Clear(void);
    
    void NextZoom(Point cursor);
    
    void UpdateClock(FAST short h, FAST short m, FAST short s)
      {
          clk.Update(h,m,s);
      }

    ~Dual(void);
};

#endif // INCDualhh
