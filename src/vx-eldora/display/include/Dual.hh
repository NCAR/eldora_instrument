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
// Revision 1.8  1994/11/29  20:29:40  thor
// Added code to handle new variable aircraft centering.
//
// Revision 1.7  1994/10/28  15:50:17  thor
// Optimized clip() for gcc.
//
// Revision 1.6  1994/04/08  20:31:52  thor
// Many changes!
//
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

    float maxAlt;
    float minAlt;
    float pixelsPerMeter;

    int firstGate;
    int xoffset;
    int radius;
    int direction;
    int offset1;
    int offset2;


    int clip(FAST int x , FAST int y)
    {
        // Yes, this produces better code from gcc then a single if.
        if (x < 0)
          return(0);
        else if (y < 0)
          return(0);
        else if (x >= Display::FULL_WIDTH)
          return(0);
        else if (y >= Display::FULL_HEIGHT/2)
          return(0);
	return(1);
    }

    int clipx(FAST int x)
    {
        // Yes, this produces better code from gcc then a single if.
        if (x < 0)
          return(0);
        else if (x >= Display::FULL_WIDTH)
          return(0);
	return(1);
    }

    int clipy(FAST int y)
    {
        // Yes, this produces better code from gcc then a single if.
        if (y < 0)
          return(0);
        else if (y >= Display::FULL_HEIGHT/2)
          return(0);
	return(1);
    }

  public:

    enum xoffsets { far_left = 0, far_right = Display::FULL_WIDTH - 1,
                    half_left = Display::FULL_WIDTH / 4, half_right =
                    (Display::FULL_WIDTH / 4) * 3, center =
                    Display::FULL_WIDTH / 2
    };

    enum sides { leftSide = 0, rightSide = 1, far_leftSide = 2,
                 far_rightSide = 4, centerSide = 8
    };
    
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
