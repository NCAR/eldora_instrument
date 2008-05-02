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
 * Revision 1.11  1993/10/27  16:04:16  thor
 * Fixed code to allow clean switch between types on the fly.
 *
 * Revision 1.10  1992/10/26  15:15:07  thor
 * Added sysInt[En|Dis]able to keep interrupts out on stops.
 *
 * Revision 1.9  1992/10/09  14:54:14  thor
 * Added LOAD_ONLY support. Modified actions on stop/start to work with new
 * Ddp code.
 *
 * Revision 1.8  1992/06/29  17:43:33  thor
 * Added code to flush pipe and reset Ddp.
 *
 * Revision 1.7  1992/02/10  16:28:53  thor
 * Added tests for empty pipe prior to flush.
 *
 * Revision 1.6  1992/02/06  21:10:29  thor
 * Add flush of pipe on restart to avoid drawing outdated data.
 *
 * Revision 1.5  1992/02/05  18:21:45  thor
 * Added new variable required for ColorConverter.
 *
 * Revision 1.4  1992/01/28  16:01:33  thor
 * Fixed typo.
 *
 * Revision 1.3  1992/01/27  18:35:54  thor
 * Added code to correct data addresses for VME/local offset.
 *
 * Revision 1.2  1992/01/22  17:54:56  thor
 * Changed to new form of ColorConverter.
 *
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

#ifdef UNIPRO
#include "VertFilter.hh"
#endif //UNIPRO

#include "Vertical.hh"
extern "C" {
#include "sysLib.h"
};

#ifdef UNIPRO
static Vertical *NewDisplay(GraphicController *, VertFilter *,
		  ColorConverter *, int);
#else
static Vertical *NewDisplay(GraphicController *, int);
#endif // UNIPRO

void VertLoop(FAST Task &self, FAST GraphicController *agc, FAST Pipe &pipe)
{
    self.FlagsInit();

    FAST Vertical *display = NULL;

#ifdef UNIPRO
    FAST ColorConverter *converter = 
      new ColorConverter(31,NULL,NULL,NULL,NULL,NULL,0,0);

    FAST VertFilter *filter = new VertFilter(1.0,0.0,.5,2.0,converter,100,936);

    FAST Pipe *fpipe = filter->Pipe();
#endif // UNIPRO

    FAST int radar;

    for (;;)
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask,FLAGS_OR);

	  switch(flag) {
	    case DESTROY_SELF:
	    case (DESTROY_SELF | NEW_DATA_FLAG):
	      if (display)
		display->Undisplay();
	      continue;
	      break;

	    case LOAD_ONLY:
	      if (GeCommand->cmd == FORWARD_VERT)
		radar = FORWARD_VERT;
	      else
		radar = AFT_VERT;
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
	      if (radar == FORWARD_VERT)
		DdpCtrl->Fore();
	      else
		DdpCtrl->Aft();
	      if (display)
		delete(display);
#ifdef UNIPRO
	      display = NewDisplay(agc,filter,converter,radar);
#else
	      display = NewDisplay(agc,radar);
#endif // UNIPRO
	      if (!pipe.Empty())
		pipe.Flush();
	      sysIntEnable(3);
	      continue;
	      break;

	    case FORWARD_VERT:
	    case (FORWARD_VERT | NEW_DATA_FLAG):
	      radar = FORWARD_VERT;
	      if (display)
		delete(display);
#ifdef UNIPRO
	      display = NewDisplay(agc,filter,converter,radar);
#else
	      display = NewDisplay(agc,radar);
#endif // UNIPRO
	      DdpCtrl->Fore();
	      if (!pipe.Empty())
		pipe.Flush();
	      continue;
	      sysIntEnable(3);
	      break;

	    case AFT_VERT:
	    case (AFT_VERT | NEW_DATA_FLAG):
	      radar = AFT_VERT;
	      if (display)
		delete(display);
#ifdef UNIPRO
	      display = NewDisplay(agc,filter,converter,radar);
#else
	      display = NewDisplay(agc,radar);
#endif // UNIPRO
	      DdpCtrl->Aft();
	      if (!pipe.Empty())
		pipe.Flush();
	      sysIntEnable(3);
	      continue;
	      break;
	  

	    case MOUSE_FLAG:
	    case (MOUSE_FLAG | NEW_DATA_FLAG):
	      VertMouse(display);
	      break;
	  }

#ifdef UNIPRO
	  DataBeam *data;
	  
	  if (pipe.QueryRead(&data))
	    {
		FAST int tmp = (int)data; // This done to correct for
		tmp += 0x30200000;	  // address difference betweem
		data = (DataBeam *)tmp;   // VMEbus & onboard memory.

		if (radar == AFT_VERT)
		  {
		      if (data->data.radar_name[0] != 'A')
			continue;
		  }
		else if (data->data.radar_name[0] != 'F')
		  continue;

		filter->Draw(*data);

	  
		while (fpipe->Empty() == FALSE)
		  {
		      VertPoint vp;
		      
		      fpipe->Read(&vp);

		      display->Draw(&vp);
		  }
	    }
#else
	  // Whatever we do for multiprocessing!
#endif //UNIPRO

	  if (pipe.Empty() == FALSE)
	    self.SetFlags(NEW_DATA_FLAG);
      }
}

#ifdef UNIPRO
static Vertical *NewDisplay(FAST GraphicController *agc,
			    FAST VertFilter *filter, 
			    FAST ColorConverter *converter,
			    FAST int rdr)
#else
static Vertical *NewDisplay(FAST GraphicController *agc, FAST int rdr)
#endif // UNIPRO
{
    FAST DispCommand *ptr = GeCommand;

    float max[3];
    float min[3];
    float scales[3];
    float biases[3];

    max[0] = ptr->max0;
    max[1] = ptr->max1;
    max[2] = ptr->max2;

    min[0] = ptr->min0;
    min[1] = ptr->min1;
    min[2] = ptr->min2;

    FAST int nv = ptr->numParams;

    FAST RADARDESC *rd = Hdr->Radar(1);

    FAST int np = rd->num_parameter_des;

    int offsets[3];

    FAST int param = ptr->param0;

    int params[3];

    if (param != NO_PARAM)
      {
	  params[0] = param;

          FAST char *ptr = ParamTapeNames[ParamToNum(param)];

          FAST int len = strlen(ptr);

          for (FAST int i = 0; i < np; i++)
            {
                PARAMETER *p = Hdr->Parameter(i);


		if (!strncmp(ptr,p->parameter_name,len))
                  {
                      offsets[0] = i;
                      scales[0] = p->parameter_scale;
                      biases[0] = p->parameter_bias;

		      if (param == VELOCITY)
			{
			    if (max[0] == 0.0 && min[0] == 0.0)
			      {
				  max[0] = rd->eff_unamb_vel;
				  min[0] = -(rd->eff_unamb_vel);
			      }
			}
                      break;
                  }
            }
      }

    param = ptr->param1;

    if (nv > 1 && param != NO_PARAM)
      {
	  params[1] = param;

          FAST char *ptr = ParamTapeNames[ParamToNum(param)];

          FAST int len = strlen(ptr);

          for (FAST int i = 0; i < np; i++)
            {
                PARAMETER *p = Hdr->Parameter(i);

                if (!strncmp(ptr,p->parameter_name,len))
                  {
                      offsets[1] = i;
		      scales[1] = p->parameter_scale;
                      biases[1] = p->parameter_bias;

		      if (param == VELOCITY)
			{
			    if (max[1] == 0.0 && min[1] == 0.0)
			      {
				  max[1] = rd->eff_unamb_vel;
				  min[1] = -(rd->eff_unamb_vel);
			      }
			}
                      break;
                  }
            }
      }

    param = ptr->param2;

    if (nv > 2 && param != NO_PARAM)
      {
	  params[2] = param;

          FAST char *ptr = ParamTapeNames[ParamToNum(param)];

          FAST int len = strlen(ptr);

          for (FAST int i = 0; i < np; i++)
            {
                PARAMETER *p = Hdr->Parameter(i);

                if (!strncmp(ptr,p->parameter_name,len))
                  {
                      offsets[2] = i;
                      scales[2] = p->parameter_scale;
                      biases[2] = p->parameter_bias;

		      if (param == VELOCITY)
			{
			    if (max[2] == 0.0 && min[2] == 0.0)
			      {
				  max[2] = rd->eff_unamb_vel;
				  min[2] = -(rd->eff_unamb_vel);
			      }
			}
                      break;
                  }
            }
      }

#ifdef UNIPRO

    converter->Reset(31,max,min,scales,biases,offsets,np,nv);

    filter->Reset(ptr->top,ptr->bottom,ptr->distance,2.0,converter,ptr->side,
		  VDATA_WIDTH);

    CELLSPACING *cs = Hdr->CellSpacing();

    converter->SetBeamSize(*cs,DISPLAYED_GATES);

    filter->Cells(*cs);

#endif //UNIPRO

    agc->setMask(0);

    agc->clear();
printf("Here\n");taskDelay(20);
    FAST Vertical *display = new Vertical(agc,nv,0,0);
printf("Here\n");taskDelay(20);
    float t = ptr->top;
    float b = ptr->bottom;
    float d = ptr->distance;

    display->SetAircraft(ptr->side,rdr,t,b,d);

    FAST u_long *colors = &GeCommand->colorTable[0];

    if (*colors != 0xffffffff)
          agc->setColorMap((long *)colors,256);

    display->DrawTable(max,min,params);

    agc->setMask(0x80);

    display->DisplaySet(A_SET);

    return(display);
}