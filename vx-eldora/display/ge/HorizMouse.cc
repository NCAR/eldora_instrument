/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 HorizMouse.cc
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
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "GeDraw.hh"

void HorizMouse(FAST HorizDisplay *disp)
{
    FAST Mouse *rodent = Rodent;

    Point cursor;

    FAST int buttonState = rodent->getButtonState();
		
    if (buttonState == LBUTTON_DOWN)
      disp->SwitchSets();
    else if (buttonState == MBUTTON_DOWN)
      {
	  cursor = rodent->getXY();
		      
	  if (cursor.x < 1024)
	    disp->NextZoom(cursor);
      }
    else if (buttonState == RBUTTON_DOWN)
      {
	  cursor = rodent->getXY();
		      
	  if (cursor.x < 1024)
	    disp->Center(cursor);
      }
    else if (buttonState == RBUTTON_DOWN | MBUTTON_DOWN)
      disp->Home();
}



    
