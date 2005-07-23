//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 HorizMisc.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.2  1994/09/27  15:45:34  thor
// Made eighth a precalculated variable. Changed name to prevent clashes.
//
// Revision 1.1  1994/04/08  20:29:53  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Horiz.hh"


void Horiz::updateLocation(DataBeam *beam)
{
    double lat = (double)beam->air.latitude;
    double lon = (double)beam->air.longitude;

    if (latitude[0] ==  (double)Horiz::IMPOSSIBLE)
      {
          double diff = degrees / 2.0;
          
          latitude[0] = lat + diff;
          latitude[1] = lat - diff;
          longitude[0] = lon - diff;
          longitude[0] = lon + diff;

          updateTitle();
          return;
      }

    int move = Horiz::NOUGHT;
    if (lat >= latitude[0])
      {
          move = Horiz::DOWN;
          latitude[0] += eighth;
          latitude[1] += eighth;
      }
    else if (lat <= latitude[1])
      {
          move = Horiz::UP;
          latitude[0] -= eighth;
          latitude[1] -= eighth;
      }
    else if (lon >= longitude[1])
      {
          move = Horiz::LEFT;
          longitude[0] += eighth;
          longitude[1] += eighth;
      }
    else if (lon <= longitude[0])
      {
          move = Horiz::RIGHT;
          longitude[0] -= eighth;
          longitude[1] -= eighth;
      }

    if (move != Horiz::NOUGHT)
      {
          updateTitle();
          shift(move);
          hashMarks((double)Horiz::DATA_WIDTH/pixelsPerMeter);
      }
}

void Horiz::shift(int direction)
{
    Point one;
    Point two;
    Point three;

    FAST int full = Horiz::DATA_WIDTH;
    FAST int eight = full / 8;
    FAST int seven = eight * 7;

    FAST int nparams = numOfParams;

    FAST Window *wdw = &Wdw[0];

    switch(direction)
      {
	case Horiz::LEFT:
	  one.x = 0;
	  one.y = 0;

	  two.x = eight;
	  two.y = 0;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(one,eight,full,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(one,eight,full,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(one,eight,full,BLACK);
		  }
	    }
	  break;
	  
	case Horiz::RIGHT:
	  one.x = eight;
	  one.y = 0;

	  two.x = 0;
	  two.y = 0;

	  three.x = seven;
	  three.y = 0;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(three,eight,full,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(three,eight,full,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(three,eight,full,BLACK);
		  }
	    }
	  break;

	case Horiz::UP:
	  one.x = 0;
	  one.y = eight;

	  two.x = 0;
	  two.y = 0;

	  three.x = 0;
	  three.y = seven;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(three,full,eight,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(three,full,eight,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(three,full,eight,BLACK);
		  }
	    }
	  break;
	  	  
	case Horiz::DOWN:
	  one.x = 0;
	  one.y = 0;

	  two.x = 0;
	  two.y = eight;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(one,full,eight,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(one,full,eight,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(one,full,eight,BLACK);
		  }
	    }
	  break;
      }
}
