//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 DisplayLoop.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.4  1994/11/01  17:49:31  thor
// Fine tuning.
//
// Revision 1.3  1994/09/23  19:58:54  thor
// Removed some debugging output.
//
// Revision 1.2  1994/09/23  15:03:57  thor
// Added clock code.
//
// Revision 1.1  1994/04/08  20:51:58  thor
// Initial revision
//
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "GeDraw.hh"

#include <sysLib.h>
#include <iostream.h>

static const int clkUpdateStart = 360; // Every 360 beams (not degrees).

void DisplayLoop(FAST Task &self, FAST Pipe &pipe)
{
    FAST int doClkUpdate = 0;
    
    FAST char radar;

    for (;;)
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask,FLAGS_OR);

          FAST volatile Display *dispPtr = display;
          
	  switch(flag)
	    {
	      case UNDISPLAY:
	      case (UNDISPLAY | NEW_DATA_FLAG):
		dispPtr->undisplay();
		continue;
		break;

	      case LOAD_ONLY:
		dispPtr = display;
		continue;
		break;

	      case STOP:
	      case (STOP | NEW_DATA_FLAG):
		sysIntDisable(3);
		DdpCtrl->Clear();
		if (!pipe.Empty())
		  {
		      pipe.Flush();
		  }
                cout << "Display stopped." << endl;
		continue;
		break;

	      case START:
	      case (START | NEW_DATA_FLAG):
		dispPtr->reset(Hdr,GeCommand);
                int type = dispPtr->getType();
                switch(type)
                  {
                    case Display::RADIAL_FORE:
                    case Display::DUAL_FORE:
                    case Display::HORIZ_FORE:
                    case Display::VERT_FORE:
                        radar = 'F';
                        DdpCtrl->Fore();
                        break;

                    case Display::RADIAL_AFT:
                    case Display::DUAL_AFT:
                    case Display::HORIZ_AFT:
                    case Display::VERT_AFT:
                        radar = 'A';
                        DdpCtrl->Aft();
                        break;
                  }

		sysIntDisable(3);
		if (!pipe.Empty())
		  pipe.Flush();
		sysIntEnable(3);
		break;
		

	      case SHOW_FORWARD:
	      case (SHOW_FORWARD | NEW_DATA_FLAG):
		radar = 'F';
		dispPtr = display;
		dispPtr->reset(Hdr,GeCommand);
		sysIntDisable(3);
		DdpCtrl->Fore();
		if (!pipe.Empty())
		  pipe.Flush();
		sysIntEnable(3);
		break;

	      case SHOW_AFT:
	      case (SHOW_AFT | NEW_DATA_FLAG):
		radar = 'A';
		dispPtr = display;
		dispPtr->reset(Hdr,GeCommand);
		sysIntDisable(3);
		DdpCtrl->Aft();
		if (!pipe.Empty())
		  pipe.Flush();
		sysIntEnable(3);
		break;

	    }
	  // Get next DDP data item.

	  if (pipe.Full())
	    {
		pipe.Flush();
		cout << "Pipe full" <<endl;
	    }

	  DataBeam *dataBeam;
	  
	  if (pipe.QueryRead(&dataBeam))
	    {
		FAST int tmp = (int)dataBeam; // This done to correct for
		tmp += 0x30200000;	      // address difference betweem
					      // VMEbus & onboard memory.
		FAST DataBeam *data = (DataBeam *)tmp;   
		
		if (data->data.radar_name[0] != radar)
                  continue;

		// Draw it.
		dispPtr->drawBeam(data);

		// Check for time update.
 		if (!doClkUpdate)
                    {
                        doClkUpdate = clkUpdateStart;
                        dispPtr->updateClock(data);
                    }
 		else
 		  --doClkUpdate;
            }
	  if (pipe.Empty() == FALSE)
	    self.SetFlags(NEW_DATA_FLAG);
      }
}
