//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 VertDraw.cc
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
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Vert.hh"

void Vert::drawBeam(DataBeam *beam)
{
    shift(beam);
    
    double dAlt = (double)altitude - beam->air.altitude_msl;

    double angle = (double)beam->air.rotation_angle;

    if (angle == 0.0 || angle == 180.0) // Obviously.
      return;
    
    FAST int side = whichSide;

    if (side == Vert::LEFT)     // Test to see if this beam is valid.
      if (angle < 180.0)
        return;
    else if (angle > 180.0)
      return;

    double heading = (double)beam->air.heading;

    if (heading == 0.0)
      heading = 360.0;

    double az = (double)beam->ray.azimuth;

    heading -= 90.0;
    
    double flatAngle = heading - az;

    if (flatAngle < 0.0)
      flatAngle = -flatAngle;

    if (flatAngle >= 90.0)
      {
          heading -= 180.0;
          flatAngle = heading - az;

          if (flatAngle < 0.0)
            flatAngle = -flatAngle;
      }

    flatAngle = DegreesToRadians(flatAngle);
    
    double elev = (double)beam->ray.elevation;
    elev = DegreesToRadians(elev);

    double dist = distance;
    double distBeam = (dist / cos(flatAngle)) / cos(elev);

    if (distBeam > maxDistance) // Over shoot.
      return;
    
    double distAlt = distBeam * sin(elev);

    double airAlt = (double)beam->air.altitude_msl;

    distAlt += airAlt;

    distAlt = altitude - distAlt; // Get y value.

    if (distAlt <= 0.0)
      return;                   // High then screen height!

    
    dist = dist * tan(flatAngle); // This yields x.

    // Code here gets rect coords.
    
    double ppm = pixelsPerMeter;

    distAlt *= ppm;

    Point gate;
    
    gate.y = (unsigned short)fastround(distAlt);

    // The x value must be added or subtracted from aircraft_x to get
    // the proper offset.

    FAST int tx = fastround(dist);
    
    if (whichSide == Vert::LEFT)
      gate.x = aircraft_x - tx;
    else
      gate.x = aircraft_x + tx;

    // Get color here!

    unsigned char data[3];

    converter->GetPoint((unsigned short *)(beam + 1),data,
			nearestGate(distBeam));

    FAST Window *wdw = Wdw;

    FAST int np = numOfParams;

    FAST unsigned char *colors = data;

    FAST int width = fastround(beamWidth * distBeam);

    gate.x -= width / 2;        // Center square.
    gate.y -= width / 2;

    do
	{
	    wdw->frect(gate,width,width,*colors++);
            wdw += 2;
	} while (--np);       
}

int Vert::nearestGate(double dist)
{
    FAST int d = fastround(dist);
    FAST int gates = maxGates;
    FAST int *cells = cellVector;
    FAST int old = 1000000;	// Impossible value.

    for (FAST int i = 0; i < maxGates; i++, cells++)
      {
	  FAST int diff = *cells - d;

	  if (diff < 0)
	      diff = -diff;

	  if (diff > old)
	      break;
	  else
	      old = diff;
      }
    return(--i);
}

void Vert::zeroCase(DataBeam *beam)
{
}

int Vert::clip(Point p, int w, int h)
{
    FAST int x = p.x;
    FAST int y = p.y;
    
    if (x < 0 || y < 0 || x >= Display::FULL_WIDTH ||
        y >= Display::FULL_HEIGHT)
      return(1);

    x += w;


    if (x >= Display::FULL_WIDTH)
      return(1);

    y += h;

    if (y >= Display::FULL_HEIGHT)
      return(1);
    
    return(0);
}

void Vert::shift(FAST DataBeam *beam)
{
    if (oldLat == -999.0)
      {
          oldLat = (double)beam->air.latitude;
          oldLong = (double)beam->air.longitude;
          return;
      }

    double lat = (double)beam->air.latitude;
    double lon = (double)beam->air.longitude;

    double x = (lat - oldLat) * Vert::METERS_PER_DEGREE;
    double y = (lon - oldLong) * Vert::METERS_PER_DEGREE;

    double z = sqrt((x * x) + (y * y));

    FAST int pixels = fastround(z * pixelsPerMeter);

    Point org = { 0, 0 };
    Point dest;

    dest.x = pixels;
    dest.y = 0;

    FAST Window *wdw = Wdw;

    FAST int np = numOfParams;
    FAST int w = Display::FULL_WIDTH - pixels;

    do
      {
          wdw->shift(org,dest,w,Display::FULL_WIDTH);
          wdw->frect(org,pixels,Display::FULL_HEIGHT,BLACK);
          wdw += 2;
      } while (--np);
}
