//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          RawDraw.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.2  1996/06/21  19:27:26  thor
// First truely working version.
//
// Revision 1.1  1996/03/25  21:57:32  thor
// Initial revision
//
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Raw.hh"
#include <string.h>
#include "fastMath.h"

#include <taskLib.h>

//Variable version
void Raw::drawBeam(FAST DataBeam *beam)
{
  // Set to end of beam.
  FAST RAW_DATA *rawHeader = (RAW_DATA *)((char *)beam + bytesPerBeam);

  if (*(int *)rawHeader != Raw::ID)
    return;
  
  if (memcmp((void *)rawHeader,(void *)&previous,sizeof(RAW_DATA)))
    {
      drawLabels(rawHeader);
      memcpy((void *)&previous,(void *)rawHeader,sizeof(RAW_DATA));
      dataPoints = rawHeader->numPoints;
      pixelsPerPoint = (double)Raw::WIDTH / (double)dataPoints;
    }
  static int drop = 0;

  if (drop < 40)
    {
      drop++;
      return;
    }
  drop = 0;
  // First clean out old data.
  Point a, b;
  a.x = 0;
  a.y = 0;
  Wdw[0].frect(a,Raw::WIDTH,Raw::HEIGHT,BLACK);

  double xinc = pixelsPerPoint;
  
  FAST int count = dataPoints;

  FAST unsigned short height = Raw::HEIGHT - 1;

  double x = 0;

  rawHeader++;
  
  FAST float *data = (float *)rawHeader;

  double vpp = constant;
  double offset = (double)previous.ymin;
  
  FAST unsigned char color = dataColor;

  a.x = 0;
  a.y = 0;

  for (FAST int i = 0; i < count; i++, x += xinc)
    {
      double datum = (double)*data++;
      b.x = (unsigned short)fastround(x);

      FAST unsigned short y = (unsigned short)fastround((datum - offset) *
                                                        vpp);

      if (y & 0x8000)
        y = 0;
      else if (y > height)
        y = height;

      y = height - y;

      b.y = y;
      
      Wdw[0].line(a,b,color);
      
      a = b;
    }
}
