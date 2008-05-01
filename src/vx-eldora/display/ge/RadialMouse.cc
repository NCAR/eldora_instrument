/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RadialMouse.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/12/02  18:05:56  thor
 * Added code to recenter cursor for zoom & pan.
 *
 * Revision 1.1  1991/10/23  20:39:56  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"

void RadialMouse(FAST Radial *disp)
{
    FAST Mouse *rodent = Rodent;

    Point cursor;

    FAST int buttonState = rodent->getButtonState();
		
    if (buttonState == LBUTTON_DOWN)
      {
	  disp->switchSets();
	  return;
      }
    else if (buttonState == MBUTTON_DOWN)
      {
	  cursor = rodent->getXY();
		      
	  if (cursor.x < PLOT_WIDTH)
	    disp->nextZoom(cursor);
      }
    else if (buttonState == RBUTTON_DOWN)
      {
	  cursor = rodent->getXY();
		      
	  if (cursor.x < PLOT_WIDTH)
//	    disp->centerAll(cursor);
	    disp->pan(cursor);
      }
    else if (buttonState == RBUTTON_DOWN | MBUTTON_DOWN)
      disp->homeAll();

    cursor.x = PLOT_WIDTH / 2;
    cursor.y = PLOT_HEIGHT / 2;

    rodent->setCursorXY(cursor);
}



    
