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
 *
 *
 * description:
 *        This contains the drawing methods for the Radial class. To
 * provide maximum speed for lesser amounts of drawn data, there are
 * three methods, one for each level of parameters.
 *
 */
#include "Dual.hh"

#ifdef NO_DISPLAY
extern "C" {
#include "taskLib.h"
};
#endif // NO_DISPLAY

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

void Dual::DrawBeam(FAST DualData &data)
{
    FAST int j = lastIndex;
    FAST int index = fastround(data.angle * 2.0);
    
    if (index < 0 || index > 720)
      {
	  printf("Angle %g out of bounds.\n",data.angle);
	  return;
      }

    float alt = data.altitude;
    float max = maxAlt;

    FAST int direction = data.direction;

    if (alt >= max && (index <= 180 || index >= 540))
      {
	  if (direction > 0)
	    index += 2;
	  else
	    index -=2;

	  lastIndex = index;

#ifdef NO_DISPLAY
	  printf("Over max\n"); taskDelay(60);
#endif // NO_DISPLAY

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
#ifdef NO_DISPLAY
	  printf("Under min\n"); taskDelay(60);
#endif // NO_DISPLAY

	  return;
      }

    if (direction > 0)
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
    
    FAST unsigned char *video1 = videoMemory[0];
    FAST unsigned char *video2 = videoMemory[1];
    
    FAST unsigned char *colors1 = (unsigned char *)&data.colors[0];
    FAST unsigned char *colors2 = colors1 + j;
    
    float ppm = pixelsPerMeter;
    float diff = (max - alt) * ppm;

    FAST int yoff;

    if (diff < 0.0)
      yoff = fastround(diff);
    else
      yoff = PLOT_HEIGHT - 1 - fastround(diff);

#ifdef NO_DISPLAY
    printf("%f %f %f %d.\n",max,alt,data.angle,yoff); taskDelay(60);
#endif // NO_DISPLAY

    FAST int fg = firstGate;

    FAST int c1 = (cos1 * fg);
    FAST int c2 = (cos2 * fg);
    FAST int s1 = (sin1 * fg);
    FAST int s2 = (sin2 * fg);
    
    FAST unsigned char *ptr1 = video1;
    FAST unsigned char *ptr2 = video2;

    if (ax > ay)
      {
	  FAST int D = ay - (ax >> 1);
	  
	  for (FAST int i = 0; i < j; i++, colors1++, colors2++,
	       c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = (c1 >> 16) + (PLOT_WIDTH / 2);
		FAST int x2 = (c2 >> 16) + (PLOT_WIDTH / 2);
		FAST int y1 = (s1 >> 16) + yoff;
		FAST int y2 = (s2 >> 16) + yoff;
		
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
	  
	  for (FAST int i = firstGate; i < j; i++, colors1++, colors2++,
	       c1 += cos1, c2 += cos2, s1 += sin1, s2 += sin2)
	    {
		FAST int x1 = c1 >> 16;
		FAST int x2 = c2 >> 16;
		FAST int y1 = s1 >> 16;
		FAST int y2 = s2 >> 16;
		
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
