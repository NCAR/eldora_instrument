//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 FieldMouse.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1994/04/08  20:56:27  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Mouse.hh"
#include "GeGlobal.hh"

void FieldMouseEvents(Task &self, FAST Mouse &rodent)
{
    for (;;)
      {
	  FAST int reason = rodent.waitOnInterrupt();

	  if (reason != MOUSE_INT)
	    continue;
	  
	  int type = display->getType();

	  Point cursor = rodent.getXY();

	  FAST int buttonState = rodent.getButtonState();

	  if (!(type & (FORWARD_DUAL | AFT_DUAL)))
	    {
		switch(buttonState)
		  {
		    case LBUTTON_DOWN:
		      display->switchSets();
		      break;

		    case MBUTTON_DOWN:
		      if (cursor.x < Display::FULL_WIDTH)
			display->nextZoom(cursor);
		      break;

		    case RBUTTON_DOWN:
		      if (cursor.x < Display::FULL_WIDTH)
			display->pan(cursor);
		      break;

		    case (RBUTTON_DOWN | MBUTTON_DOWN):
		      display->home();
		      break;
		  }
		cursor.y = Display::FULL_HEIGHT / 2;
	    }
	  else
	    {
		switch(buttonState)
		  {
		    case MBUTTON_DOWN:
		      if (cursor.x < Display::FULL_WIDTH)
			display->nextZoom(cursor);
		      break;

		    case RBUTTON_DOWN:
		      if (cursor.x < Display::FULL_WIDTH)
			display->pan(cursor);
		      break;

		    case (RBUTTON_DOWN | MBUTTON_DOWN):
		      display->home();
		      break;
		  }
		cursor.y = Display::FULL_HEIGHT / 4;  
	    }
	  cursor.x = Display::FULL_WIDTH / 2;
          
	  rodent.setCursorXY(cursor);
      }
}
