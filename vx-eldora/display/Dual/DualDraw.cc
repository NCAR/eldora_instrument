/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 DualDraw.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.7  1994/04/11  17:52:43  thor
// Fixed bug that had display gates starting before first gate!
//
// Revision 1.6  1994/04/08  20:31:30  thor
// Many changes!
//
 * Revision 1.5  1993/10/21  16:38:57  thor
 * Temp save of latest work.
 *
 * Revision 1.4  1993/09/28  13:12:27  thor
 * Uncommented over case.
 *
 * Revision 1.3  1993/09/03  17:10:25  thor
 * Fixed consts to avoid clashing with others.
 *
 * Revision 1.2  1993/08/31  16:39:41  thor
 * The first really working version.
 *
 * Revision 1.1  1992/11/06  20:20:23  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This contains the drawing methods for the Radial class. To
 * provide maximum speed for lesser amounts of drawn data, there are
 * three methods, one for each level of parameters.
 *
 */
#include "Dual.hh"


void Dual::drawBeam(FAST DataBeam *beam)
{
    double angle = (double)beam->air.rotation_angle;
    double roll = (double)beam->air.roll;
    
    angle += roll;
    
    if (angle < 0.0)
      angle += 360.0;
    else if (angle > 360.0)
      angle -= 360.0;

    FAST int j = lastIndex;
    FAST int index = fastround(angle * 2.0);

    if (index == 720)
      index = 0;
    
    if (index < 0 || index > 720)
      {
	  cout << "Angle " << angle << "out of bounds." << endl;
	  return;
      }

    float alt = beam->air.altitude_msl * 1000.0;
    float max = maxAlt;

    FAST int direction = fastround(beam->ray.true_scan_rate);

    if (alt >= max && (index <= 180 || index >= 540))
      {
	  if (direction > 0)
	    index += 2;
	  else
	    index -=2;

	  lastIndex = index;

	  return;
      }

    float min = minAlt;

    if (alt <= min && (index > 180 && index < 540))
      {
	  if (direction > 0)
	    index += 2;
	  else
	    index -=2;

	  lastIndex = index;

	  return;
      }

    if (direction > 0)
      {
	  // This is positive rotation.
	  
	  if (j == 1000)
	    j = index - 2;

          FAST int q;

          if (index)
            q = index - j;
          else
            q = 720 - j;

          if (q < 0)
            q = -q;

          q = 720 - q;
          
          if (q < 700)
            {
                cout << "Angle too big - " << index << "," << j << endl;
                lastIndex = index;
                return;
            }
	  
	  index += 2;
	  
	  if (j == index)
	    j -= 4;
	  
	  if (j < 0)
	    j += 720;
	  
	  if (index >= 720)
	    index -= 720;
      }
    else
      {
	  // This is negative rotation.
	  
	  if (j == 1000)
	    j = index + 2;

	  FAST int q;

          if (index)
            q = index - j;
          else
            q = 720 - j;

          if (q < 0)
            q = -q;

          q = 720 - q;
          
          if (q < 700)
            {
                cout << "Angle too big - " << index << "," << j << endl;
                lastIndex = index;
                return;
            }

	  index -= 2;
	  
	  if (j == index)
	    j += 4;
	  
	  if (j >= 720)
	    j -= 720;
	  
	  if (index < 0)
	    index += 720;
      }
    
    lastIndex = index;
    FAST int sin1 = trigData[j].sin;
    FAST int cos1 = trigData[j].cos;
    
    FAST int cos2 = trigData[index].cos;
    FAST int sin2 = trigData[index].sin;    
    
    j = radius;
    
    FAST int dx = ((j * cos2) >> 16) - ((j * cos1) >> 16);
    FAST int dy = ((j * sin2) >> 16) - ((j * sin1) >> 16);
    
    FAST int ax = (dx < 0 ? -dx : dx) << 1;
    FAST int ay = (dy < 0 ? -dy : dy) << 1;
    
    FAST int sx = dx < 0 ? -1 : 1;
    FAST int sy = dy < 0 ? -1 : 1;
    
    FAST unsigned char *video1 = videoMemory[0];
    FAST unsigned char *video2 = videoMemory[1];

    unsigned char cvalues[2048];

    converter->GetBeam((unsigned short *)(beam + 1),cvalues);
    
    float ppm = pixelsPerMeter;

    alt = max - alt;
    
    FAST int yoff = fastround(alt * ppm);

    FAST int fg = firstGate;

    FAST unsigned char *colors1 = cvalues + fg;
    FAST unsigned char *colors2 = cvalues + 1024;
    
    FAST int c1 = (cos1 * fg);
    FAST int c2 = (cos2 * fg);
    FAST int s1 = (sin1 * fg);
    FAST int s2 = (sin2 * fg);

    FAST unsigned char *ptr1 = video1;
    FAST unsigned char *ptr2 = video2;

    if (ax > ay)
      {
	  FAST int D = ay - (ax >> 1);
	  
	  for (FAST int i = fg; i < j; i++, colors1++,
	       colors2++, c1 += cos1, c2 += cos2, s1 += sin1,
	       s2 += sin2)
	    {
		FAST int x1 = (c1 >> 16) + (Display::FULL_WIDTH / 2);
		FAST int x2 = (c2 >> 16) + (Display::FULL_WIDTH / 2);
		FAST int y1 = (s1 >> 16) + yoff;
		FAST int y2 = (s2 >> 16) + yoff;

		if (!clip(x1,y1) && !clip(x2,y2)) // Both points outside of
		  continue;			  // drawing area!
		
		FAST int d = D;
		FAST int x = x1;
		FAST int xend = x2;
		FAST int Sx = sx;
		FAST int Ax = ax;
		FAST int Ay = ay;
		
		if (sy == 1)
		  {
		      for (;;)
			{
			    if (!clip(x,y1)) break;
			    plot(ptr1,x,y1,*colors1);
			    plot(ptr2,x,y1,*colors2);
			    
			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  y1++;
				  if (!clip(x,y1)) break;
				  plot(ptr1,x,y1,*colors1);
				  plot(ptr2,x,y1,*colors2);
				  d -= Ax;
			      }
			    x += Sx;
			    d += Ay;
			}
		  }
		else
		  {
		      for (;;)
			{
			    if (!clip(x,y1)) break;
			    plot(ptr1,x,y1,*colors1);
			    plot(ptr2,x,y1,*colors2);
			    
			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  y1--;
				  if (!clip(x,y1)) break;
				  plot(ptr1,x,y1,*colors1);
				  plot(ptr2,x,y1,*colors2);
				  d -= Ax;
			      }
			    x += Sx;
			    d += Ay;
			}
		  }
	    }
      }
    else
      {
	  
	  FAST int D = ax - (ay >> 1);
	  
	  for (FAST int i = fg; i < j; i++, colors1++,
	       colors2++, c1 += cos1, c2 += cos2, s1 += sin1,
	       s2 += sin2)
	    {
		FAST int x1 = (c1 >> 16) + (Display::FULL_WIDTH / 2);
		FAST int x2 = (c2 >> 16) + (Display::FULL_WIDTH / 2);
		FAST int y1 = (s1 >> 16) + yoff;
		FAST int y2 = (s2 >> 16) + yoff;
		
		if (!clip(x1,y1) && !clip(x2,y2)) // Both points outside of
		  continue;			  // drawing area!
		
		FAST int d = D;
		FAST int y = y1;
		FAST int yend = y2;
		
		if (sy == 1)
		  {
		      for (;;) 
			{
			    if (!clip(x1,y)) break;
			    plot(ptr1,x1,y,*colors1);
			    plot(ptr2,x1,y,*colors2);
			    
			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  x1 += sx;
				  if (!clip(x1,y)) break;
				  plot(ptr1,x1,y,*colors1);
				  plot(ptr2,x1,y,*colors2);
				  d -= ay;
			      }   
			    y++;
			    d += ax;
			}
		  }
		else
		  {
		      for (;;) 
			{
			    if (!clip(x1,y)) break;
			    plot(ptr1,x1,y,*colors1);
			    plot(ptr2,x1,y,*colors2);
    
			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  x1 += sx;
				  if (!clip(x1,y)) break;
				  plot(ptr1,x1,y,*colors1);
				  plot(ptr2,x1,y,*colors2);
				  d -= ay;
			      }
			    y--;
			    d += ax;
			}
		  }
	    }
      }
}
