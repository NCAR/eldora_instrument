//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 HorizDraw.cc
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

#include "Horiz.hh"

void Horiz::drawBeam(DataBeam *beam)
{
    updateLocation(beam);
    
    double dAlt = (double)altitude - (beam->air.altitude_msl * 1000.0);
    double elev = (double)beam->ray.elevation;
cout << dAlt << " " << elev << endl;
    if (elev == 0.0 && dAlt == 0.0)
      {
	  zeroCase(beam);
          return;
      }

    if (dAlt > 0.0)             // Positive, so elevation must be as well.
      {
          if (elev < 0.0)
            {
                cout << "Under " << elev << endl;
                return;
            }
      }
    else if (dAlt < 0.0)        // Negative.
      {
          if (elev > 0.0)
            {
                cout << "Over" << endl;
                return;
            }

          elev = -elev;		// Rationalize to positive case.
          dAlt = -dAlt;
      }

    double plusOne = elev + 1.0; // Get beam sides.
    double minusOne = elev - 1.0;

    elev = DegreesToRadians(elev); // Convert to radians.
    plusOne = DegreesToRadians(plusOne);
    minusOne = DegreesToRadians(minusOne);

    double currDist = dAlt / sin(elev);

    if (currDist > maxDistance)
      return;                   // Too far out!

    double ppm = pixelsPerMeter;

    dAlt *= ppm;                // Convert to pixels.
    currDist *= ppm;

    FAST int x1 = fastround(dAlt / tan(plusOne)); // Get x cooord. for sides.
    FAST int x2 = fastround(dAlt / tan(minusOne));

    int height = x2 - x1;

    // This is the width of the beam at our distance.
    int width = fastround(beamWidth * currDist);

    // Now get corrected x,y location.
    double azimuth = beam->ray.azimuth;

    cout << "az = " << azimuth << endl;
    FAST int index = fastround((azimuth - 1.0) * 2.0);

    FAST int sine = trigData[index].sin;
    FAST int cosine = trigData[index].cos;

    FAST int x = (x2 * cosine) >> 16;
    FAST int y = (x2 * sine) >> 16;

    double lat = (double)beam->air.latitude;
    double lon = (double)beam->air.longitude;

    lat = latitude[0] - lat;
    lon = longitude[0] - lon;

    lat *= ppm * METERS_PER_DEGREE;
    lon *= ppm * METERS_PER_DEGREE;

    int ax = fastround(lat);
    int ay = fastround(lon);
    
    x += ax;
    y += ay;
    
    Point gate;

    gate.x = (unsigned short)x;
    gate.y = (unsigned short)y;

    Point air;

    air.x = (unsigned short)ax;
    air.y = (unsigned short)ay;

    if (clip(gate,width,height))             // Drawing off screen!
      {
          cout << "clipped " << gate.x << "," << gate.y << endl;
          return;
      }
    // Get color here!

    unsigned char data[3];

    converter->GetPoint((unsigned short *)(beam + 1),data,
			nearestGate(currDist));

    FAST Window *wdw = Wdw;

    FAST int np = numOfParams;

    FAST unsigned char *colors = data;

    cout << gate.x << "," << gate.y << " " << air.x << ",";
    cout << air.y << endl;
//     do
// 	{
// 	    wdw->frect(gate,width,height,*colors++);
//             wdw->point(air,WHITE1);
//             wdw += 2;
// 	} while (--np);       
}

int Horiz::nearestGate(double dist)
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

void Horiz::zeroCase(DataBeam *beam)
{
    FAST int ngates = maxGates;
    double ppm = pixelsPerMeter;
    FAST int *cv = cellVector;
    FAST unsigned short *points = (unsigned short *)(beam + 1);

    // Now get corrected x,y location.
    double azimuth = beam->ray.azimuth;

    FAST int index = fastround((azimuth - 1.0) * 2.0);

    FAST int sine = trigData[index].sin;
    FAST int cosine = trigData[index].cos;

    double lat = (double)beam->air.latitude;
    double lon = (double)beam->air.longitude;

    lat = latitude[0] - lat;
    lon = longitude[0] - lon;

    lat *= ppm * METERS_PER_DEGREE;
    lon *= ppm * METERS_PER_DEGREE;

    Point air;
    Point gate;

    air.x = (unsigned short)(fastround(lat));
    air.y = (unsigned short)(fastround(lon));

    for (FAST int i = 0; i < ngates; i++)
      {
          double dist = (double)*cv++;
          
          FAST int width = fastround(beamWidth * dist * ppm);

          Point gate;
          gate.x = ((-(width / 2) * cosine) >> 16) + air.x;
          gate.y = (fastround(dist * ppm) >> 16) + air.y;

          FAST int height = fastround(((double)*cv - dist) * ppm);

          if (clip(gate,width,height))
            continue;
          
          unsigned char data[3];
          
          converter->GetPoint(points,data,i);

          FAST Window *wdw = Wdw;

          FAST int np = numOfParams;

          FAST unsigned char *colors = data;
    
          do
            {
                wdw->frect(gate,width,height,*colors++);
                wdw->point(air,WHITE1);
                wdw += 2;
            } while (--np);       
      }
}

int Horiz::clip(Point p, int w, int h)
{
    FAST int x = p.x;
    FAST int y = p.y;
    
    if (x < 0 || y < 0 || x >= Horiz::DATA_WIDTH ||
        y >= Horiz::DATA_HEIGHT)
      return(1);

    x += w;


    if (x >= Horiz::DATA_WIDTH)
      return(1);

    y += h;

    if (y >= Horiz::DATA_HEIGHT)
      return(1);
    
    return(0);
}
