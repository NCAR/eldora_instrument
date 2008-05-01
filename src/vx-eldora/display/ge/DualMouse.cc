/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 DualMouse.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1993/09/28  13:19:09  thor
// Initial revision
//
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "GeDraw.hh"

void DualMouse(FAST Dual *disp)
{
    FAST Mouse *rodent = Rodent;

    Point cursor;

    FAST int buttonState = rodent->getButtonState();
		
    if (buttonState == MBUTTON_DOWN)
      {
	  cursor = rodent->getXY();
		      
	  if (cursor.x < PLOT_WIDTH)
	    disp->NextZoom(cursor);
      }
    else if (buttonState == RBUTTON_DOWN)
      {
	  cursor = rodent->getXY();

	  if (cursor.x < PLOT_WIDTH)
	    disp->Pan(cursor);
      }
    else if (buttonState == RBUTTON_DOWN | MBUTTON_DOWN)
      disp->Home();

    cursor.x = PLOT_WIDTH / 2;
    cursor.y = PLOT_HEIGHT / 4;

    rodent->setCursorXY(cursor);
}



    
