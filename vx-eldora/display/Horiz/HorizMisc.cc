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

    double eighth = degrees / 8.0;

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
    FAST int eighth = full / 8;
    FAST int seven = eighth * 7;

    FAST int nparams = numOfParams;

    FAST Window *wdw = &Wdw[0];

    switch(direction)
      {
	case Horiz::LEFT:
	  one.x = 0;
	  one.y = 0;

	  two.x = eighth;
	  two.y = 0;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(one,eighth,full,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(one,eighth,full,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(one,eighth,full,BLACK);
		  }
	    }
	  break;
	  
	case Horiz::RIGHT:
	  one.x = eighth;
	  one.y = 0;

	  two.x = 0;
	  two.y = 0;

	  three.x = seven;
	  three.y = 0;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(three,eighth,full,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(three,eighth,full,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(three,eighth,full,BLACK);
		  }
	    }
	  break;

	case Horiz::UP:
	  one.x = 0;
	  one.y = eighth;

	  two.x = 0;
	  two.y = 0;

	  three.x = 0;
	  three.y = seven;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(three,full,eighth,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(three,full,eighth,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(three,full,eighth,BLACK);
		  }
	    }
	  break;
	  	  
	case Horiz::DOWN:
	  one.x = 0;
	  one.y = 0;

	  two.x = 0;
	  two.y = eighth;

	  wdw->shift(one,two,seven,full);
	  wdw->frect(one,full,eighth,BLACK);
	  
	  if (nparams > 1)
	    {
		wdw += 2;
		wdw->shift(one,two,seven,full);
		wdw->frect(one,full,eighth,BLACK);
		
		if (nparams > 2)
		  {
		      wdw += 2;
		      wdw->shift(one,two,seven,full);
		      wdw->frect(one,full,eighth,BLACK);
		  }
	    }
	  break;
      }
}
