/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 VertMouse.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/12/18  20:36:07  thor
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

void VertMouse(FAST Vertical *disp)
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
	    disp->Center(cursor);
      }
    else if (buttonState == RBUTTON_DOWN | MBUTTON_DOWN)
      disp->Home();

    cursor.x = PLOT_WIDTH / 2;
    cursor.y = PLOT_HEIGHT / 2;

    rodent->setCursorXY(cursor);
}



    
