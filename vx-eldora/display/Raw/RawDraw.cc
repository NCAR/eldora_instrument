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
      pixelsPerPoint = Raw::WIDTH / dataPoints;
    }
  // First clean out old data.
  Point a, b;
  a.x = 0;
  a.y = 0;
  Wdw[0].frect(a,Raw::WIDTH,Raw::HEIGHT,BLACK);

  FAST unsigned short xinc = pixelsPerPoint;
  
  FAST int count = dataPoints;

  FAST unsigned short height = Raw::HEIGHT - 1;

  FAST unsigned short x = 0;

  rawHeader++;
  
  FAST float *data = (float *)rawHeader;

  double vpp = 999.0;

  FAST unsigned char color = dataColor;

  a.x = 0;
  a.y = 0;
  
  for (FAST int i = 0; i < count; i++, x += xinc)
    {
      double datum = *data++;
      b.x = x;
      b.y = height - (unsigned short)fastround(datum * vpp);

      Wdw[0].line(a,b,color);
      
      a = b;
    }
}
