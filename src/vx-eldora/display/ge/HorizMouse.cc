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
 * Revision 1.2  1991/12/02  19:27:25  thor
 * Added code to center cursor for zoom & pan.
 *
 * Revision 1.1  1991/10/28  19:37:39  thor
 * Initial revision
 *
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
      {
	  disp->SwitchSets();
	  return;
      }
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
	    disp->Pan(cursor);
      }
    else if (buttonState == RBUTTON_DOWN | MBUTTON_DOWN)
      disp->Home();

    cursor.x = PLOT_WIDTH / 2;
    cursor.y = PLOT_HEIGHT / 2;

    rodent->setCursorXY(cursor);
}



    