//
//	$Id$
//
//	Module:		 Radial.hh
//	Original Author: Richard E. K. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 2.6  1994/09/19  16:02:03  thor
// Major rewrite.
//
// Revision 2.5  1993/11/22  16:58:30  thor
// Added code to support resetting.
//
// Revision 2.4  1993/10/27  14:37:09  thor
// Added Undisplay().
//
// Revision 2.3  1993/09/03  17:16:54  thor
// Forgot the !!@## define line.
//
// Revision 2.2  1993/09/03  17:14:39  thor
// Added protection around trigData.
//
// Revision 2.1  1993/07/29  19:21:37  thor
// Upgraded to VxWorks 5.1.
//
// Revision 2.0  1992/11/03  12:52:47  thor
// First offical ELDORA release!
//
// Revision 1.7  1991/10/24  13:48:03  thor
// Switched order of window actions in C_SET part of switchSets so
// A_SET is displayed.
//
// Revision 1.6  1991/10/23  14:42:43  thor
// Added some needed includes.
//
// Revision 1.5  1991/10/09  18:03:28  thor
// Added forward class definition.
//
// Revision 1.4  1991/06/13  17:50:29  thor
// Add references to rpc headers. Added SetBounds method.
// Changes drawTitle method to use radar type instead of parameter.
//
// Revision 1.3  1991/05/06  15:11:45  thor
// Added embedded Clock class for time display.
//
// Revision 1.2  1991/04/29  18:46:53  thor
// Changed calling of drawing routines to use new RadialData format.
//
// Revision 1.1  1991/04/08  20:29:04  thor
// Initial revision
//
//
// description:
//        The class Radial implements drawing of an RHI type display.
// It can currently draw up to 3 parameters. Methods are provided to
// draw a color table with associated units, title text, hash marks &
// text and most of the drawing & window operations available from the
// Window class. Methods are also available to display the window
// sets. These are the logical pairing of a data & table window. An
// embedded clock is provided via the Clock class member. This clock
// must be manually updated.
//
//
#ifndef INCRadialhh
#define INCRadialhh
#pragma interface

class Radial;

#include "Display.hh"

class Radial :public Display {
  private:
    unsigned char *videoMemory[3];
    unsigned char *videoBase[3];

    unsigned char data[DISPLAYED_GATES * 3];

    int radius;
    int firstGate;

    float maxDistance;

    void Draw1(int sin1, int sin2, int cos1, int cos2);
    void Draw2(int sin1, int sin2, int cos1, int cos2);
    void Draw3(int sin1, int sin2, int cos1, int cos2);

  public:
    Radial(GraphicController *gbd);

    void reset(Header *hdr, DispCommand *cmd);

    void displaySet(int set);
    
    void drawBeam(DataBeam *beam);
    
    void drawTitle(int set, int radar);
    
    ~Radial(void);

};

#endif INCRadialhh
