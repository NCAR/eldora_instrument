//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Raw.hh
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1996/03/25  21:57:33  thor
// Initial revision
//
//
//
//
// description:
//        
//
#ifndef _RAW_HH
#define _RAW_HH

#pragma interface

class Raw;

#include "Display.hh"
#include "raw_data.h"

class Raw: public Display {
  public:
  Raw(GraphicController *gbd);

  void setcolor(unsigned char color) { dataColor = color; }

  void reset(Header *hdr, DispCommand *cmd);

  void displaySet(int set);
    
  void drawBeam(DataBeam *beam);
    
  void drawTitle(int set, int radar);

  void undisplay();

  // The following are included to prevent these operations from happening.
  void switchSets();

  private:

  enum screensizes { WIDTH = 1000, HEIGHT = 1000, MAX_WIDTH = 1280,
  ID = 0x52415744 };

  void drawLabels(RAW_DATA *header);
  
  int dataPoints;
  double pixelsPerPoint;        // How far along the x axis.
  int bytesPerBeam;             // So we know how far to jump.
  double constant;              // Converts values to pixels.

  RAW_DATA previous;

  unsigned char dataColor;
  
  unsigned short commandStore[Raw::WIDTH];
};

#endif // _RAW_HH
