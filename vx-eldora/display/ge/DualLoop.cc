/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 DualLoop.cc
 *	Original Author: Richard E. Neitzel
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

#include "sysLib.h"
#include "fastMath.h"

static Dual *makeDisplay(Dual *, GraphicController *);

#include "ColorConverter.hh"
static ColorConverter *conv = NULL;

static int whichRadar = FORWARD_DUAL;

void DualLoop(FAST Task &self, FAST GraphicController *agc, FAST Pipe &pipe)
{
    self.FlagsInit();

    FAST Dual *display = NULL;

    float lastAngle = 360.0;	// Force update of time.

    FAST int radar = FORWARD_DUAL;

    static int onetime = 0;

    for (;;)
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask,FLAGS_OR);

	  switch(flag)
	    {
	      case DESTROY_SELF:
	      case (DESTROY_SELF | NEW_DATA_FLAG):
		if (display != NULL)
		  display->Undisplay();
		continue;
		break;

	      case LOAD_ONLY:
		if (GeCommand->cmd == FORWARD_DUAL)
		  radar = FORWARD_DUAL;
		else
		  radar = AFT_DUAL;

		whichRadar = radar;
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
		continue;
		break;

	      case START:
	      case RELOAD:
	      case RESTART_DISP:
	      case (RELOAD | NEW_DATA_FLAG):
	      case (START | NEW_DATA_FLAG):
	      case (RESTART_DISP | NEW_DATA_FLAG):
		if (radar == FORWARD_DUAL)
		  DdpCtrl->Fore();
		else
		  DdpCtrl->Aft();
		display = makeDisplay(display,agc);
		lastAngle = 360.0;
		sysIntEnable(3);
		break;

	      case FORWARD_DUAL:
	      case (FORWARD_DUAL | NEW_DATA_FLAG):
		whichRadar = FORWARD_DUAL;
		radar = whichRadar;
		display = makeDisplay(display,agc);
		lastAngle = 360.0;
		sysIntDisable(3);
		DdpCtrl->Fore();
		if (!pipe.Empty())
		  pipe.Flush();
		sysIntEnable(3);
		break;

	      case AFT_DUAL:
	      case (AFT_DUAL | NEW_DATA_FLAG):
		whichRadar = AFT_DUAL;
		radar = whichRadar;
		display = makeDisplay(display,agc);
		lastAngle = 360.0;
		sysIntDisable(3);
		DdpCtrl->Aft();
		if (!pipe.Empty())
		  pipe.Flush();
		sysIntEnable(3);
		break;

	      case MOUSE_FLAG:
	      case (MOUSE_FLAG | NEW_DATA_FLAG):
		DualMouse(display);
		break;
	    }
	  // Get next DDP data item.

	  DataBeam *dataBeam;

	  if (pipe.Full())
	    {
		pipe.Flush();
		printf("Pipe full\n");
	    }

	  if (pipe.QueryRead(&dataBeam))
	    {
		FAST int tmp = (int)dataBeam; // This done to correct for
		tmp += 0x30200000;	      // address difference betweem
		dataBeam = (DataBeam *)tmp;   // VMEbus & onboard memory.

		if (radar == AFT_DUAL)
		  {
		      if (dataBeam->data.radar_name[0] != 'A')
			continue;
		  }
		else if (dataBeam->data.radar_name[0] != 'F')
		  continue;

		DualData dualData;

		FAST Beam_Time *now = &dualData.time;

		memcpy((char *)now,(char *)&(dataBeam->ray.hour),
				sizeof(short) * 4);

		memcpy((char *)LastTime,(char *)now,
		      sizeof(Beam_Time));

		float ra = dataBeam->air.rotation_angle;
		float roll = dataBeam->air.roll;

		ra -= roll;

		if (ra < 0.0)
                  ra += 360.0;
                else if (ra > 360.0)
                  ra -= 360.0;

		dualData.angle = ra;

		FAST int direct = fastround(dataBeam->ray.true_scan_rate);

		dualData.direction = direct;
		dualData.altitude = dataBeam->air.altitude_msl * 1000.0;

		dualData.data = (unsigned short *)(dataBeam + 1);

		if (direct >= 0)
		  {
		      if (lastAngle >= dualData.angle) // Crossed to
						      // next sweep.
			display->UpdateClock(now->hour,now->minute,
					     now->second);
		  }
		else
		  {
		      if (lastAngle < dualData.angle)
			display->UpdateClock(now->hour,now->minute,
					     now->second);
		  }

		lastAngle = dualData.angle;

		// Draw it.
		display->DrawBeam(dualData);

		if (pipe.Empty() == FALSE)
		  {
		      self.SetFlags(NEW_DATA_FLAG);
		      continue;
		  }
	    }
      }
}

static Dual *makeDisplay(FAST Dual *old, FAST GraphicController *agc)
{
    FAST DispCommand *ptr = GeCommand;

    if (old != NULL)
      delete(old);   

    // The following is only for uniprocessor systems!!!!!
    if (conv != NULL)
      delete(conv);

    float max[2];
    float min[2];
    float scales[2];
    float biases[2];

    max[0] = ptr->max0;
    max[1] = ptr->max1;

    min[0] = ptr->min0;
    min[1] = ptr->min1;

    FAST RADARDESC *rd = Hdr->Radar(1);

    FAST int np = rd->num_parameter_des;

    int offsets[2];

    FAST int param = ptr->param0;

    FAST char *name = ParamTapeNames[ParamToNum(param)];

    FAST int len = strlen(name);

    for (FAST int i = 0; i < np; i++)
      {
	  PARAMETER *p = Hdr->Parameter(i);

	  if (!strncmp(name,p->parameter_name,len))
	    {
		offsets[0] = i;
		scales[0] = p->parameter_scale;
		biases[0] = p->parameter_bias;
		break;
	    }
      }

    param = ptr->param1;

    name = ParamTapeNames[ParamToNum(param)];

    len = strlen(name);

    for (i = 0; i < np; i++)
      {
	  PARAMETER *p = Hdr->Parameter(i);
	  
	  if (!strncmp(name,p->parameter_name,len))
	    {
		offsets[1] = i;
		scales[1] = p->parameter_scale;
		biases[1] = p->parameter_bias;
		break;
	    }
      }

    int nv = ptr->numParams;
    
    // Again uniprocessor only!
    conv = new ColorConverter(31,max,min,scales,biases,offsets,np,nv);

    agc->setMask(0);

    agc->clear();

    FAST Dual *New = new Dual(agc,0,0);

    FAST u_long *colors = &GeCommand->colorTable[0];

    if (*colors != 0xffffffff)
	  agc->setColorMap((long *)colors,256);

    CELLSPACING *cs = Hdr->CellSpacing();

    New->Converter(conv);
    New->SetBounds(*cs,(float)ptr->top,(float)ptr->bottom);
    
    FAST int wdw = 0;

    param = ptr->param0;
    New->DrawTable(A_SET,max[0],min[0],param);

    param = ptr->param1;
    New->DrawTable(B_SET,max[1],min[1],param);
    New->DrawTitle();

    agc->setMask(0x80);

    New->Display();

    return(New);
}
