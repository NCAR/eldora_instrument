/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RadialDraw.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:52:47  thor
 * First offical ELDORA release!
 *
 * Revision 1.12  1992/10/21  14:30:44  thor
 * This time I think the fat beam stuff is right.
 *
 * Revision 1.11  1992/10/16  14:12:33  thor
 * Added fat beam code to 1 & 3 parameter methods.
 *
 * Revision 1.10  1992/10/09  14:58:58  thor
 * Added code to reject overly large beams.
 *
 * Revision 1.9  1992/02/05  20:44:33  thor
 * Converted multiplication to addition for more speed.
 *
 * Revision 1.8  1992/02/03  21:33:24  thor
 * Added fast inline version of iround.
 *
 * Revision 1.7  1992/02/03  20:23:28  thor
 * Switched to faster iround function from (int)float_value. Moved another
 * constant out of loop.
 *
 * Revision 1.6  1992/01/22  14:43:52  thor
 * Moved invariant code out of main loop & moved main loop inward.
 *
 * Revision 1.5  1991/11/19  14:08:28  thor
 * Added trap for out of bounds angles.
 *
 * Revision 1.4  1991/11/06  20:25:58  thor
 * Changed direction test to use positive/negative values.
 *
 * Revision 1.3  1991/06/13  17:53:56  thor
 * Changed main loop to start at first real gate.
 *
 * Revision 1.2  1991/04/29  18:48:34  thor
 * Changed logic to determine sides to take positive/negative rotaunt.
 *
 * Revision 1.1  1991/04/08  20:29:20  thor
 * Initial revision
 *
 *
 * description:
 *        This contains the drawing methods for the Radial class. To
 * provide maximum speed for lesser amounts of drawn data, there are
 * three methods, one for each level of parameters.
 *
 */
#include "Radial.hh"
static inline int fastround(double d);

static inline int fastround(double d)
{
    register int i;
    register int cm;
    register int im;

    asm volatile ("fmove%.l fpcr,%0" : "=dm" (cm) : );

    im = cm & 0xff00;

    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (im));

    asm volatile ("fmove%.l %1,%0" : "=d" (i) : "f" (d));

    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (cm));

    return(i);
}

void Radial::Draw1(FAST RadialData &data)
{
    FAST int j = lastIndex;
    FAST int index = fastround(data.angle * 2.0);

    if (index < 0 || index > 720)
      {
	  printf("Angle %g out of bounds.\n",data.angle);
	  return;
      }

    if (data.direction > 0)
      {
	  // This is positive rotation.

	  if (j == 1000)
	    j = index - 2;

	  FAST int q = index - j;

	  if (q < -20)
	    q = -q;

	  if (q > 20)
	    {
		printf("Angle to big, %d %d\n",index,j);
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

	  FAST int q = index - j;

	  if (q < -20)
	    q = -q;

	  if (q > 20)
	    {
		printf("Angle to big, %d %d\n",index,j);
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

    FAST unsigned char *video1 = videoBase[0];

    FAST unsigned char *colors1 = (unsigned char *)&data.colors[0];

    FAST int fg = firstGate;

    FAST int c1 = cos1 * fg;
    FAST int c2 = cos2 * fg;
    FAST int s1 = sin1 * fg;
    FAST int s2 = sin2 * fg;

    if (ax > ay)
      {
	  for (FAST int i = firstGate; i < j; i++, colors1++,
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
	  for (FAST int i = firstGate; i < j; i++, colors1++,
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

void Radial::Draw2(FAST RadialData &data)
{
    FAST int j = lastIndex;
    FAST int index = fastround(data.angle * 2.0);

    if (index < 0 || index > 720)
      {
	  printf("Angle %g out of bounds.\n",data.angle);
	  return;
      }

    if (data.direction > 0)
      {
	  // This is positive rotation.

	  if (j == 1000)
	    j = index - 2;

	  FAST int q = index - j + 720;


	  if (q < 700)
	    {
		printf("Angle to big, %d %d\n",index,j);
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

	  FAST int q = index - j;

	  q = 720 - q;

	  if (q < 700)
	    {
		printf("Angle to big, %d %d\n",index,j);
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

    FAST unsigned char *video1 = videoBase[0];
    FAST unsigned char *video2 = videoBase[1];

    FAST unsigned char *colors1 = (unsigned char *)&data.colors[0];
    FAST unsigned char *colors2 = colors1 + j;

    FAST int fg = firstGate;

    FAST int c1 = cos1 * fg;
    FAST int c2 = cos2 * fg;
    FAST int s1 = sin1 * fg;
    FAST int s2 = sin2 * fg;

    if (ax > ay)
      {
	  FAST int D = ay - (ax >> 1);

	  for (FAST int i = firstGate; i < j; i++, colors1++, colors2++,
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

	  for (FAST int i = firstGate; i < j; i++, colors1++, colors2++,
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

void Radial::Draw3(FAST RadialData &data)
{
    FAST int j = lastIndex;
    FAST int index = fastround(data.angle * 2.0);

    if (index < 0 || index > 720)
      {
	  printf("Angle %g out of bounds.\n",data.angle);
	  return;
      }

    if (data.direction > 0)
      {
	  // This is positive rotation.

	  if (j == 1000)
	    j = index - 2;

	  FAST int q = index - j;

	  if (q < -20)
	    q = -q;

	  if (q > 20)
	    {
		printf("Angle to big, %d %d\n",index,j);
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

	  FAST int q = index - j;

	  if (q < -20)
	    q = -q;

	  if (q > 20)
	    {
		printf("Angle to big, %d %d\n",index,j);
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

    FAST unsigned char *video1 = videoBase[0];
    FAST unsigned char *video2 = videoBase[1];
    FAST unsigned char *video3 = videoBase[2];

    FAST unsigned char *colors1 = (unsigned char *)&data.colors[0];
    FAST unsigned char *colors2 = colors1 + j;
    FAST unsigned char *colors3 = colors2 + j;

    FAST int fg = firstGate;

    FAST int c1 = cos1 * fg;
    FAST int c2 = cos2 * fg;
    FAST int s1 = sin1 * fg;
    FAST int s2 = sin2 * fg;

    if (ax > ay)
      {
	  for (FAST int i = firstGate; i < j; i++, colors1++, colors2++,
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
	  for (FAST int i = firstGate; i < j; i++, colors1++, colors2++,
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
