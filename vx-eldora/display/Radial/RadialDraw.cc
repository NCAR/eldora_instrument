//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 RadialDraw.cc
//	Original Author: Richard E. K. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 2.2  1994/07/06  14:13:25  thor
// Fixed roll direction.
//
// Revision 2.1  1993/10/27  14:37:09  thor
// Fixed too big an angle code.
//
// Revision 2.0  1992/11/03  12:52:47  thor
// First offical ELDORA release!
//
// Revision 1.12  1992/10/21  14:30:44  thor
// This time I think the fat beam stuff is right.
//
// Revision 1.11  1992/10/16  14:12:33  thor
// Added fat beam code to 1 & 3 parameter methods.
//
// Revision 1.10  1992/10/09  14:58:58  thor
// Added code to reject overly large beams.
//
// Revision 1.9  1992/02/05  20:44:33  thor
// Converted multiplication to addition for more speed.
//
// Revision 1.8  1992/02/03  21:33:24  thor
// Added fast inline version of iround.
//
// Revision 1.7  1992/02/03  20:23:28  thor
// Switched to faster iround function from (int)float_value. Moved another
// constant out of loop.
//
// Revision 1.6  1992/01/22  14:43:52  thor
// Moved invariant code out of main loop & moved main loop inward.
//
// Revision 1.5  1991/11/19  14:08:28  thor
// Added trap for out of bounds angles.
//
// Revision 1.4  1991/11/06  20:25:58  thor
// Changed direction test to use positive/negative values.
//
// Revision 1.3  1991/06/13  17:53:56  thor
// Changed main loop to start at first real gate.
//
// Revision 1.2  1991/04/29  18:48:34  thor
// Changed logic to determine sides to take positive/negative rotaunt.
//
// Revision 1.1  1991/04/08  20:29:20  thor
// Initial revision
//
//
// description:
//        This contains the drawing methods for the Radial class. To
// provide maximum speed for lesser amounts of drawn data, there are
// three methods, one for each level of parameters.
//
//
#include "Radial.hh"

void Radial::drawBeam(FAST DataBeam *beam)
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
    
    if (index < 0 || index > maxIndex)
      {
	  cout << "Angle " << angle << "out of bounds." << endl;
	  return;
      }

    FAST int j = lastIndex;
    FAST int direction = fastround(beam->ray.true_scan_rate);
    
    if (direction >= 0)
      {
	  // This is positive (or no) rotation.

	  if (j == 1000)
	    j = index - 2;

	  FAST int q;

          if (index)
            q = index - j;
          else
            q = maxIndex - j;

	  if (q < -20)
	    q = -q;

          q = maxIndex - q;
          
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

	  if (q < -20)
	    q = -q;

          q = maxIndex - q;

	  if (q < 700)
	    {
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

    FAST int np = numOfParams;

    converter->GetBeam((unsigned short *)(beam + 1),data);
    
    if (np == 1)
      Draw1(sin1,sin2,cos1,cos2);
    else if (np == 2)
      Draw2(sin1,sin2,cos1,cos2);
    else
      Draw3(sin1,sin2,cos1,cos2);
}

void Radial::Draw1(FAST int sin1, FAST int sin2, FAST int cos1, FAST int cos2)
{
    FAST int j = radius;

    FAST int dx = ((j * cos2) >> 16) - ((j * cos1) >> 16);
    FAST int dy = ((j * sin2) >> 16) - ((j * sin1) >> 16);

    FAST int ax = (dx < 0 ? -dx : dx) << 1;
    FAST int ay = (dy < 0 ? -dy : dy) << 1;

    FAST int sx = dx < 0 ? -1 : 1;
    FAST int sy = dy < 0 ? -1 : 1;

    FAST unsigned char *video1 = videoBase[0];

    FAST int fg = firstGate;
    
    FAST unsigned char *colors1 = data + fg;

    FAST int c1 = cos1 * fg;
    FAST int c2 = cos2 * fg;
    FAST int s1 = sin1 * fg;
    FAST int s2 = sin2 * fg;

    if (ax > ay)
      {
	  for (FAST int i = fg; i < j; i++, colors1++,
	       c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;

		FAST int d = ay - (ax >> 1);
		FAST unsigned char *ptr1 = video1 + x1 + (y1 * 4096);
		FAST int x = x1;
		FAST int xend = x2;
		FAST int Sx = sx;
		FAST int Ax = ax;
		FAST int Ay = ay;

		if (sy == 1)
		  {
		      for (;;)
			{
			    *ptr1 = *colors1;
			    
			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 += 4096;
				  *ptr1 = *colors1;
				  d -= Ax;
			      }
			    x += Sx;
			    ptr1 += Sx;
			    d += Ay;
			}
		  }
		else
		  {
		      for (;;)
			{
			    *ptr1 = *colors1;

			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 -= 4096;
				  *ptr1 = *colors1;
				  d -= Ax;
			      }
			    x += Sx;
			    ptr1 += Sx;
			    d += Ay;
			}
		  }
	    }
      }
    else
      {
	  for (FAST int i = fg; i < j; i++, colors1++,
	       c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;

		FAST int d = ax - (ay >> 1);
		FAST unsigned char *ptr1 = video1 + x1 + (y1 * 4096);
		FAST int y = y1;
		FAST int yend = y2;

		if (sy == 1)
		  {
		      for (;;) 
			{
			    *ptr1 = *colors1;

			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 += sx;
				  *ptr1 = *colors1;
				  d -= ay;
			      }

			    y++;
			    ptr1 += 4096;
			    d += ax;
			}
		  }
		else
		  {
		      for (;;) 
			{
			    *ptr1 = *colors1;

			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 += sx;
				  *ptr1 = *colors1;
				  d -= ay;
			      }

			    y--;
			    ptr1 -= 4096;
			    d += ax;
			}
		  }
	    }
      }
}		

void Radial::Draw2(FAST int sin1, FAST int sin2, FAST int cos1, FAST int cos2)
{
    FAST int j = radius;

    FAST int dx = ((j * cos2) >> 16) - ((j * cos1) >> 16);
    FAST int dy = ((j * sin2) >> 16) - ((j * sin1) >> 16);

    FAST int ax = (dx < 0 ? -dx : dx) << 1;
    FAST int ay = (dy < 0 ? -dy : dy) << 1;

    FAST int sx = dx < 0 ? -1 : 1;
    FAST int sy = dy < 0 ? -1 : 1;

    FAST unsigned char *video1 = videoBase[0];
    FAST unsigned char *video2 = videoBase[1];

    FAST int fg = firstGate;
    
    FAST unsigned char *colors1 = data + fg;
    FAST unsigned char *colors2 = colors1 + j;

    FAST int c1 = cos1 * fg;
    FAST int c2 = cos2 * fg;
    FAST int s1 = sin1 * fg;
    FAST int s2 = sin2 * fg;

    if (ax > ay)
      {
	  FAST int D = ay - (ax >> 1);

	  for (FAST int i = fg; i < j; i++, colors1++, colors2++,
	       c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;

		FAST int d = D;
		FAST unsigned char *ptr1 = video1 + x1 + (y1 * 4096);
		FAST unsigned char *ptr2 = video2 + x1 + (y1 * 4096);
		FAST int x = x1;
		FAST int xend = x2;
		FAST int Sx = sx;
		FAST int Ax = ax;
		FAST int Ay = ay;

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
				  ptr1 += 4096;
				  ptr2 += 4096;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  d -= Ax;
			      }
			    x += Sx;
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
				  ptr1 -= 4096;
				  ptr2 -= 4096;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  d -= Ax;
			      }
			    x += Sx;
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

	  for (FAST int i = fg; i < j; i++, colors1++, colors2++,
	       c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;

		FAST int d = D;
		FAST unsigned char *ptr1 = video1 + x1 + (y1 * 4096);
		FAST unsigned char *ptr2 = video2 + x1 + (y1 * 4096);
		FAST int y = y1;
		FAST int yend = y2;

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
				  ptr1 += sx;
				  ptr2 += sx;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  d -= ay;
			      }

			    y++;
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
				  ptr1 += sx;
				  ptr2 += sx;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  d -= ay;
			      }

			    y--;
			    ptr1 -= 4096;
			    ptr2 -= 4096;
			    d += ax;
			}
		  }
	    }
      }
}		

void Radial::Draw3(FAST int sin1, FAST int sin2, FAST int cos1, FAST int cos2)
{
    FAST int j = radius;

    FAST int dx = ((j * cos2) >> 16) - ((j * cos1) >> 16);
    FAST int dy = ((j * sin2) >> 16) - ((j * sin1) >> 16);

    FAST int ax = (dx < 0 ? -dx : dx) << 1;
    FAST int ay = (dy < 0 ? -dy : dy) << 1;

    FAST int sx = dx < 0 ? -1 : 1;
    FAST int sy = dy < 0 ? -1 : 1;

    FAST unsigned char *video1 = videoBase[0];
    FAST unsigned char *video2 = videoBase[1];
    FAST unsigned char *video3 = videoBase[2];

    FAST int fg = firstGate;

    FAST unsigned char *colors1 = data + fg;
    FAST unsigned char *colors2 = colors1 + j;
    FAST unsigned char *colors3 = colors2 + j;

    FAST int c1 = cos1 * fg;
    FAST int c2 = cos2 * fg;
    FAST int s1 = sin1 * fg;
    FAST int s2 = sin2 * fg;

    if (ax > ay)
      {
	  for (FAST int i = fg; i < j; i++, colors1++, colors2++,
	       colors3++, c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;

		FAST int d = ay - (ax >> 1);
		FAST unsigned char *ptr1 = video1 + x1 + (y1 * 4096);
		FAST unsigned char *ptr2 = video2 + x1 + (y1 * 4096);
		FAST unsigned char *ptr3 = video3 + x1 + (y1 * 4096);
		FAST int x = x1;
		FAST int xend = x2;
		FAST int Sx = sx;
		FAST int Ax = ax;
		FAST int Ay = ay;

		if (sy == 1)
		  {
		      for (;;)
			{
			    *ptr1 = *colors1;
			    *ptr2 = *colors2;
			    *ptr3 = *colors3;
			    
			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 += 4096;
				  ptr2 += 4096;
				  ptr3 += 4096;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  *ptr3 = *colors3;
				  d -= Ax;
			      }
			    x += Sx;
			    ptr1 += Sx;
			    ptr2 += Sx;
			    ptr3 += Sx;
			    d += Ay;
			}
		  }
		else
		  {
		      for (;;)
			{
			    *ptr1 = *colors1;
			    *ptr2 = *colors2;
			    *ptr3 = *colors3;

			    if (x == xend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 -= 4096;
				  ptr2 -= 4096;
				  ptr3 -= 4096;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  *ptr3 = *colors3;
				  d -= Ax;
			      }
			    x += Sx;
			    ptr1 += Sx;
			    ptr2 += Sx;
			    ptr3 += Sx;
			    d += Ay;
			}
		  }
	    }
      }
    else
      {
	  for (FAST int i = fg; i < j; i++, colors1++, colors2++,
	       colors3++, c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;

		FAST int d = ax - (ay >> 1);
		FAST unsigned char *ptr1 = video1 + x1 + (y1 * 4096);
		FAST unsigned char *ptr2 = video2 + x1 + (y1 * 4096);
		FAST unsigned char *ptr3 = video3 + x1 + (y1 * 4096);
		FAST int y = y1;
		FAST int yend = y2;

		if (sy == 1)
		  {
		      for (;;) 
			{
			    *ptr1 = *colors1;
			    *ptr2 = *colors2;
			    *ptr3 = *colors3;

			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 += sx;
				  ptr2 += sx;
				  ptr3 += sx;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  *ptr3 = *colors3;
				  d -= ay;
			      }

			    y++;
			    ptr1 += 4096;
			    ptr2 += 4096;
			    ptr3 += 4096;
			    d += ax;
			}
		  }
		else
		  {
		      for (;;) 
			{
			    *ptr1 = *colors1;
			    *ptr2 = *colors2;
			    *ptr3 = *colors3;

			    if (y == yend)
			      break;
			    
			    if (d >= 0)
			      {
				  ptr1 += sx;
				  ptr2 += sx;
				  ptr3 += sx;
				  *ptr1 = *colors1;
				  *ptr2 = *colors2;
				  *ptr3 = *colors3;
				  d -= ay;
			      }

			    y--;
			    ptr1 -= 4096;
			    ptr2 -= 4096;
			    ptr3 -= 4096;
			    d += ax;
			}
		  }
	    }
      }
}		
