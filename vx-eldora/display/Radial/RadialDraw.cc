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
 *
 * Changed main loop to start at first real gate.
 *
 * Revision 1.2  1991/04/29  18:48:34  thor
 * Changed logic to determine sides to take positive/negative rotaunt.
 *
 * Revision 1.1  1991/04/08  20:29:20  thor
 * Initial revision

void Radial::Draw1(float angle, FAST RadialData *data)

    FAST int index = (int)(angle * 2.0);

    asm volatile ("fmove%.l %1,%0" : "=d" (i) : "f" (d));
    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (cm));
// This is based on positive rotation for now.
    if (j == 1000)
      j = index - 2;
    if (index < 0 || index > 720)
    index += 2;
	  return;
    if (j == index)
      j -= 4;

    if (j < 0)
      j += 720;
	  // This is positive rotation.
    if (index >= 720)
      index -= 720;
	    j = index - 2;
	    index -= 720;
      }
    else
      {
	  // This is negative rotation.

	  if (j == 1000)
	    j = index + 2;

		return;
	    }
    FAST unsigned char *colors1 = (unsigned char *)data;
	  index -= 2;
    for (FAST int i = 0; i < j; i++, colors1++)
	    index += 720;
	  FAST int x1 = (i * cos1) >> 16;
	  FAST int x2 = (i * cos2) >> 16;
	  FAST int y1 = (i * sin1) >> 16;
	  FAST int y2 = (i * sin2) >> 16;
	  
	  FAST int dx = x2 - x1;
	  FAST int dy = y2 - y1;

	  FAST int ax = (dx < 0 ? -dx : dx) << 1;
	  FAST int ay = (dy < 0 ? -dy : dy) << 1;
    FAST int sin2 = trigData[index].sin;    
	  FAST int sx = dx < 0 ? -1 : 1;
	  FAST int sy = dy < 0 ? -1 : 1;
	  
	  if (ax > ay)
	    {

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
	  else
				  d -= Ax;
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
void Radial::Draw2(float angle, FAST RadialData *data)
			      }
    FAST int index = (int)(angle * 2.0);


			    ptr1 += 4096;
// This is based on positive rotation for now.
    if (j == 1000)
      j = index - 2;
			    if (y == yend)
    index += 2;
			    if (d >= 0)
    if (j == index)
      j -= 4;
				  ptr1 += sx;
    if (j < 0)
      j += 720;
			      }
    if (index >= 720)
      index -= 720;
			    ptr1 -= 4096;
	  if (j == 1000)
	    j = index - 2;

	  FAST int q = index - j;

	  if (q < -20)
	    q = -q;

	  if (q > 20)
	    j -= 4;

	  if (j < 0)
    FAST unsigned char *colors1 = (unsigned char *)data;

	  if (index >= 720)
    for (FAST int i = 0; i < j; i++, colors1++, colors2++)

	  FAST int x1 = (i * cos1) >> 16;
	  FAST int x2 = (i * cos2) >> 16;
	  FAST int y1 = (i * sin1) >> 16;
	  FAST int y2 = (i * sin2) >> 16;
	  
	  FAST int dx = x2 - x1;
	  FAST int dy = y2 - y1;

	  FAST int ax = (dx < 0 ? -dx : dx) << 1;
	  FAST int ay = (dy < 0 ? -dy : dy) << 1;

	  FAST int sx = dx < 0 ? -1 : 1;
	  FAST int sy = dy < 0 ? -1 : 1;
	  
	  if (ax > ay)
	    {
		FAST int d = ay - (ax >> 1);

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
	  else
		if (sy == 1)
		FAST int d = ax - (ay >> 1);
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
void Radial::Draw3(float angle, FAST RadialData *data)
		FAST int yend = y2;
    FAST int index = (int)(angle * 2.0);


			    if (y == yend)
// This is based on positive rotation for now.
    if (j == 1000)
      j = index - 2;
			      {
    index += 2;
				  *ptr1 = *colors1;
    if (j == index)
      j -= 4;
				  d -= ay;
    if (j < 0)
      j += 720;
			    y++;
    if (index >= 720)
      index -= 720;
			    d += ax;
			    ptr1 -= 4096;
			    ptr2 -= 4096;
			    d += ax;
			}
		  }
	    }
      }
}		

      }

    if (data.direction > 0)
      {
    FAST unsigned char *colors1 = (unsigned char *)data;

	  if (j == 1000)

    for (FAST int i = 0; i < j; i++, colors1++, colors2++, colors3++)
		printf("Angle to big, %d %d\n",index,j);
	  FAST int x1 = (i * cos1) >> 16;
	  FAST int x2 = (i * cos2) >> 16;
	  FAST int y1 = (i * sin1) >> 16;
	  FAST int y2 = (i * sin2) >> 16;
	  
	  FAST int dx = x2 - x1;
	  FAST int dy = y2 - y1;
	    j -= 4;
	  FAST int ax = (dx < 0 ? -dx : dx) << 1;
	  FAST int ay = (dy < 0 ? -dy : dy) << 1;

	  FAST int sx = dx < 0 ? -1 : 1;
	  FAST int sy = dy < 0 ? -1 : 1;
	  
	  if (ax > ay)
	    {

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
	  else
      {
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
