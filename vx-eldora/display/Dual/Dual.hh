//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Dual.hh
//	Original Author: Richard E. K. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.5  1993/10/21  16:38:57  thor
// Temp save of latest work.
//
// Revision 1.4  1993/09/03  17:18:15  thor
// Forgot the !##$$# define line.
//
// Revision 1.4  1993/09/03  17:18:15  thor
// Forgot the !##$$# define line.
//
// Revision 1.3  1993/09/03  17:10:25  thor
// Fixed consts to avoid clashing with others.
//
// Revision 1.2  1993/08/31  16:39:41  thor
// The first really working version.
//
// Revision 1.1  1992/11/06  20:20:24  thor
// Initial revision
//
//
//
// description:
//        
//
#ifndef INCDualhh
#define INCDualhh
#pragma interface

class Dual;

// These defines are needed for rpc header.
#define OK_RPC
#define WRS_SIDE


#include "Display.hh"

class Dual : public Display {
  private:
    unsigned char *videoMemory[2];

    int firstGate;

    float maxAlt;
    float minAlt;
    float pixelsPerMeter;
    int radius;

    int direction;

    int offset1;
    int offset2;


    int clip(FAST int x , FAST int y)
    {
	if ((x < 0) || (x >= Display::FULL_WIDTH) || (y < 0) ||
	    (y >= (Display::FULL_HEIGHT/ 2)))
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
    Dual(GraphicController *gbd);

    void reset(Header *hdr, DispCommand *cmd);

    void undisplay(void);
    
    void drawBeam(DataBeam *beam);
    
    void drawTable(int set, float max, float min, int param,
                   int tblsize = 31);
    
    void drawTitle(int set, int radar);

    void displaySet(int set);

    void switchSets(void);

    ~Dual(void);
};

#endif // INCDualhh
