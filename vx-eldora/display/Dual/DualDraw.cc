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
// Revision 1.12  1994/11/29  20:29:40  thor
// Added code to handle new variable aircraft centering & simplified
// end of drawing tests.
//
// Revision 1.11  1994/11/01  15:25:59  thor
// Let's try a faster(?) way to draw.
//
// Revision 1.10  1994/10/27  16:54:04  thor
// Fixed incorrect offset for 2nd set of colors.
//
// Revision 1.9  1994/09/27  17:24:19  thor
// Changed some floating point calculations to integer.
//
// Revision 1.8  1994/07/06  13:57:41  thor
// Fixed roll direction.
//
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
    
    FAST int index = fastround(angle * 2.0);
    FAST int maxIndex = 720;
    
    if (index < 0)
      index += maxIndex;
    else if (index > maxIndex)
      index -= maxIndex;
    else if (index == maxIndex)
      index = 0;
    
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

    FAST int j = lastIndex;
    
    if (direction > 0)
      {
	  // This is positive rotation.
	  
	  if (j == 1000)
	    j = index - 2;

          FAST int q;

          if (index)
            q = index - j;
          else
            q = maxIndex - j;

          if (q < 0)
            q = -q;

          q = maxIndex - q;
          
          if (q < 700)
            {
                if (q)
                  cout << "Angle too big - " << index << "," << j << endl;
                lastIndex = index;
                return;
            }
	  
	  index += 2;
	  
	  if (j == index)
	    j -= 4;
	  
	  if (j < 0)
	    j += maxIndex;
	  
	  if (index >= maxIndex)
	    index -= maxIndex;
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
            q = maxIndex - j;

          if (q < 0)
            q = -q;

          q = maxIndex - q;
          
          if (q < 700)
            {
                if (q)
                  cout << "Angle too big - " << index << "," << j << endl;
                lastIndex = index;
                return;
            }

	  index -= 2;
	  
	  if (j == index)
	    j += 4;
	  
	  if (j >= maxIndex)
	    j -= maxIndex;
	  
	  if (index < 0)
	    index += maxIndex;
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
    
    unsigned char cvalues[2048];

    converter->GetBeam((unsigned short *)(beam + 1),cvalues);
    
    float ppm = pixelsPerMeter;

    alt = max - alt;
    
    FAST int yoff = fastround(alt * ppm);

    FAST int fg = firstGate;

    FAST unsigned char *colors1 = cvalues + fg;
    FAST unsigned char *colors2 = colors1 + 1024;
    
    FAST int c1 = (cos1 * fg);
    FAST int c2 = (cos2 * fg);
    FAST int s1 = (sin1 * fg);
    FAST int s2 = (sin2 * fg);

    FAST unsigned char *video1 = videoMemory[0];
    FAST unsigned char *video2 = videoMemory[1];

    FAST int xoff = xoffset;

    if (ax > ay)
      {
	  FAST int D = ay - (ax >> 1);
	  
	  for (FAST int i = fg; i < j; i++, colors1++,
	       colors2++, c1 += cos1, c2 += cos2, s1 += sin1,
	       s2 += sin2)
	    {
		FAST int x1 = (c1 >> 16) + xoff;
		FAST int x2 = (c2 >> 16) + xoff;
		FAST int y1 = (s1 >> 16) + yoff;
		FAST int y2 = (s2 >> 16) + yoff;

		if (!clip(x1,y1)) // Yes this will leave some ragged edges,
		  break;        // but we want speed!

		FAST int d = D;
		FAST int x = x1;
		FAST int xend = x2;
		FAST int Sx = sx;
		FAST int Ax = ax;
		FAST int Ay = ay;
                FAST unsigned char *ptr1 = video1 + x + (y1 * 4096);
                FAST unsigned char *ptr2 = video2 + x + (y1 * 4096);

		if (sy == 1)
		  {
		      for (;;)
			{
                            *ptr1 = *colors1;
			    *ptr2 = *colors2;
			    
			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  y1++;
				  if (y1 >= Display::FULL_HEIGHT / 2)
                                    break;
                                  ptr1 += 4096;
                                  ptr2 += 4096;
                                  *ptr1 = *colors1;
                                  *ptr2 = *colors2;
				  d -= Ax;
			      }
			    x += Sx;
                            if (!clipx(x))
                              break;
                            ptr1 += Sx;
                            ptr2 += Sx;
			    d += Ay;
			}
		  }
		else
		  {
		      for (;;)
			{
                            *ptr1 = *colors1;
                            *ptr2 = *colors2;
			    
			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  y1--;
				  if (y1 < 0)
                                    break;
                                  ptr1 -= 4096;
                                  ptr2 -= 4096;
				  *ptr1 = *colors1;
                                  *ptr2 = *colors2;
				  d -= Ax;
			      }
			    x += Sx;
                            if (!clipx(x))
                              break;
                            ptr1 += Sx;
                            ptr2 += Sx;
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
		FAST int x1 = (c1 >> 16) + xoff;
		FAST int x2 = (c2 >> 16) + xoff;
		FAST int y1 = (s1 >> 16) + yoff;
		FAST int y2 = (s2 >> 16) + yoff;
		
                if (!clip(x1,y1)) // Yes this will leave some ragged edges,
		  break;        // but we want speed!
		
		FAST int d = D;
		FAST int y = y1;
		FAST int yend = y2;
                FAST unsigned char *ptr1 = video1 + x1 + (y * 4096);
                FAST unsigned char *ptr2 = video2 + x1 + (y * 4096);

		if (sy == 1)
		  {
		      for (;;) 
			{
                            *ptr1 = *colors1;
                            *ptr2 = *colors2;
			    
			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  x1 += sx;
				  if (!clipx(x1)) break;
                                  ptr1 += sx;
                                  ptr2 += sx;
                                  *ptr1 = *colors1;
                                  *ptr2 = *colors2;
				  d -= ay;
			      }   
			    y++;
                            if (y >= Display::FULL_HEIGHT / 2)
                              break;
                            ptr1 += 4096;
                            ptr2 += 4096;
			    d += ax;
			}
		  }
		else
		  {
		      for (;;) 
			{
                            *ptr1 = *colors1;
                            *ptr2 = *colors2;
    
			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  x1 += sx;
                                  if (!clipx(x1))
                                    break;
                                  ptr1 += sx;
                                  ptr2 += sx;
                                  *ptr1 = *colors1;
                                  *ptr2 = *colors2;
				  d -= ay;
			      }
			    y--;
                            if (y < 0)
                              break;
                            ptr1 -= 4096;
                            ptr2 -= 4096;
			    d += ax;
			}
		  }
	    }
      }
}
