/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/10/14  20:33:44  thor
 * Added code to compute number of variables, parameters, etc. and plug them
 *
 * Revision 1.1  1991/10/14  19:22:18  thor
 * Initial revision
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *
 *
 * description:
 *        
    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (cm));

// The following are only for uniprocessor systems!!!
static char whichRadar = 'F';

#include "ColorConveter.hh"
static ColorConverter *conv = NULL;
static Radial *makeDisplay(Radial *, GraphicController *);
void RadialLoop(FAST Task &self, FAST GraphicController *agc)
#include "ColorConverter.hh"
static ColorConverter *conv = NULL;

    FAST unsigned int flag = self.WaitOnFlags(waitMask,FLAGS_OR);

    Radial *display = NULL;

    FAST Radial *display = NULL;

	  if (flag)

		switch(flag)
		  {
		    case STOP:
		      if (display != NULL)
			delete(display);
		      flag = self.WaitOnFlags(waitMask,FLAGS_OR);
		      continue;
		      break;

		    case START:
		    case RELOAD:
		    case RESTART:
		    case FORWARD_RADIAL:
		    case AFT_RADIAL:
		      display = makeDisplay(display,agc);
		      break;
		if (!pipe.Empty())
		    default:
		      continue;
		      break;
		  }
	      case (AFT_RADIAL | NEW_DATA_FLAG):
		whichRadar = AFT_RADIAL;
		radar = whichRadar;
	  // Color convert it - only on uniprocessor!

	  // Draw it.

	  flag = self.QueryFlags();
			display->UpdateClock(now->hour,now->minute,
					     now->second);
		  }

		lastAngle = radData.angle;
		display->drawBeam(radData);

		if (pipe.Empty() == FALSE)
		  {
		      self.SetFlags(NEW_DATA_FLAG);
		      continue;
    int offsets[3];
      }
}

static Radial *makeDisplay(FAST Radial *old, FAST GraphicController *agc)
{
    FAST DispCommand *ptr = GeCommand;

    if (old != NULL)
      delete(old);   
    conv = new ColorConverter(DISPLAYED_GATES,max,min,offsets,
			      x,ptr->numParams);
//
    if (old != NULL)
      delete(old);

    FAST DispCommand *ptr = GeCommand;

    FAST Radial *New = Radial(agc,DISPLAYED_GATES,ptr->numParams,0,0);
      {
	  param = ptr->param1;
	  New->drawTable(B_SET,max[1],min[1],param);
	  New->drawTitle(B_SET,whichRadar);
      }

    if (nv > 2)
      {
	  param = ptr->param2;
	  New->drawTable(C_SET,max[2],min[2],param);
	  New->drawTitle(C_SET,whichRadar);
      }

    agc->setMask(0x80);

    New->setPriority(1);

    New->displaySet(A_SET);

    return(New);
}
