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
 * Revision 1.8  1991/11/19  17:54:32  thor
 * Added code to select forward/aft beams only.
 *
 * Revision 1.7  1991/11/19  17:17:23  thor
 * Put color change after new.
 *
 * Revision 1.6  1991/11/15  16:57:06  thor
 * Added new radar variable and removed test taskDelay.
 *
 * Revision 1.5  1991/11/06  21:44:24  thor
 * Removed bunch of test code.
 * Added real data handling & scaling.
 *
 * Revision 1.4  1991/11/01  20:06:38  thor
 * Added support for scaling max/min.
 *
 * Revision 1.3  1991/11/01  20:02:49  thor
 * Added support for reading from pipe and setting time.
 *
 * Revision 1.2  1991/10/30  17:56:38  thor
 * First working version.
 *
 * Revision 1.1  1991/10/28  19:37:49  thor
 * Initial revision
 *
 *
 *
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

// Uniprocessor conditional.
#define UNIPRO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "GeDraw.hh"

static HorizDisplay *makeDisplay(HorizDisplay *, GraphicController *);

#include "ColorConverter.hh"
static ColorConverter *conv = NULL;
static Horiz *horizFilter = NULL;

static int whichRadar = FORWARD_HORIZ;

void HorizLoop(FAST Task &self, FAST GraphicController *agc, FAST Pipe &pipe)
{
    self.FlagsInit();

    FAST HorizDisplay *display = NULL;
    Pipe dataPipe(sizeof(DataPoint),100);
    Pipe movePipe(sizeof(HorizMove),10);




    Pipe dataPipe(sizeof(HorizPoint),100);

	  FAST unsigned int flag = self.WaitOnFlags(waitMask | NEW_DATA_FLAG |
						    DESTROY_SELF | MOUSE_FLAG,
      {

	  FAST unsigned int flag = self.WaitOnFlags(waitMask | TMO_FLAG,
						    FLAGS_OR);

	  switch(flag)
	    {
	      case DESTROY_SELF:
	      case (DESTROY_SELF | NEW_DATA_FLAG):
		if (display != NULL)
		  {
		      delete(display);
		      display = NULL;
		  }
		if (horizFilter != NULL) // Only the filter need be
					 // deleted, as it deleltes
					 // the converter.
		  {
		      delete(horizFilter);
		      horizFilter = NULL;
		  }
		continue;
		break;

	      case LOAD_ONLY:
		if (GeCommand->cmd == FORWARD_RADIAL)
		  radar = FORWARD_RADIAL;
		else
		  radar = AFT_RADIAL;

		whichRadar = radar;
		continue;
		break;
		horizFilter->Pipe(&dataPipe,&movePipe);
	      case STOP:
		continue;
		break;

	      case START:
	      case RELOAD:
	      case RESTART:
	      case (RELOAD | NEW_DATA_FLAG):
		horizFilter->Pipe(&dataPipe,&movePipe);
	      case (RESTART | NEW_DATA_FLAG):
		reset = 1;
                if (!pipe.Empty())
		  pipe.Flush();
		break;

	      case FORWARD_HORIZ:
	      case (FORWARD_HORIZ | NEW_DATA_FLAG):
		horizFilter->Pipe(&dataPipe,&movePipe);
                radar = whichRadar;
		break;

	      case AFT_HORIZ:
	      case (AFT_HORIZ | NEW_DATA_FLAG):
		whichRadar = AFT_HORIZ;
                radar = whichRadar;
		break;

	      case TMO_FLAG:
	      case (TMO_FLAG | NEW_DATA_FLAG):
		{
		    FAST DispCommand *ptr = GeCommand;
		    FAST int tmo = ptr->tmo;
		      display->StartTimer(tmo);
		}
	    }

	  DataBeam *dataBeam;

	  // Get next DDP data item.
	  if (pipe.QueryRead(&dataBeam))

		      horizFilter->ResetCoord(dataBeam->air.latitude,
					  dataBeam->air.longitude);

		      HorizMove m;

		      m.direction = INITIAL_LAT_LONG;
		      m.latitude = dataBeam->air.latitude;
		      m.longitude = dataBeam->air.longitude;

		      display->Shift(&m);
		  }

		FAST Beam_Time *now = (Beam_Time *)&(dataBeam->ray.hour);

		// Check for moves.
		while (movePipe.Empty() == FALSE)
		  {
		      HorizMove m;
		      
		      movePipe.Read(&m);
		if (++count == 10)
		      display->Shift(&m);
		  }
		  {
		      display->UpdateClock(now->hour,now->minute,now->second);
		      count = 0;
		      DataPoint dp;

		bcopy((char *)now,(char *)LastTime,
		      sizeof(Beam_Time));


		// Color convert it - only on uniprocessor!
		// This is hard! 
		horizFilter->Draw(*dataBeam);


	  self.SetFlags(NEW_DATA_FLAG);
		// Draw it.
		while (dataPipe.Empty() == FALSE)
		  {
		      HorizPoint dp;

		      dataPipe.Read(&dp);

		      display->Draw(&dp);
		  }

		if (pipe.Empty() == FALSE)
		  self.SetFlags(NEW_DATA_FLAG);
	    }
      }
}

static HorizDisplay *makeDisplay(FAST HorizDisplay *old,
				 FAST GraphicController *agc)
{
    float Max[3];
    float Min[3];
    if (old != NULL)
      delete(old);   

    // The following is only for uniprocessor systems!!!!!
    if (horizFilter != NULL)
      {
	  delete(horizFilter);
      }

    bcopy((char *)max,(char *)Max,sizeof(float) * 3);
    bcopy((char *)min,(char *)Min,sizeof(float) * 3);

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

		      max[0] = (max[0] - p->parameter_bias) / 
			p->parameter_scale;
		      min[0] = (min[0] - p->parameter_bias) / 
			p->parameter_scale;
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
		      break;
		  }
	    }
      }

    param = ptr->param1;

		      max[1] = (max[1] - p->parameter_bias) / 
			p->parameter_scale;
		      min[1] = (min[1] - p->parameter_bias) / 
			p->parameter_scale;
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
		      break;
		  }
	    }
      }

    param = ptr->param2;

		      max[2] = (max[2] - p->parameter_bias) / 
			p->parameter_scale;
		      min[2] = (min[2] - p->parameter_bias) / 
			p->parameter_scale;
	  FAST char *ptr = ParamTapeNames[ParamToNum(param)];

	  FAST int len = strlen(ptr);

	  for (FAST int i = 0; i < np; i++)
	    {
    conv = new ColorConverter(DISPLAYED_GATES,max,min,offsets,np,nv);
    horizFilter = new Horiz(0.0,0.0,ptr->distance,MAX_RECT,DATA_WIDTH,*conv,
			    EldoraBeam);
                      scales[2] = p->parameter_scale;
                      biases[2] = p->parameter_bias;
		      break;
		  }
	    }
      }

    // Again uniprocessor only!
    conv = 
      new ColorConverter(DISPLAYED_GATES,max,min,scales,biases,offsets,np,nv);

    horizFilter = new Horiz(0.0,0.0,ptr->distance,ptr->size/1000.0,DATA_WIDTH,
			    *conv,EldoraBeam);

    FAST CELLSPACING *cs = Hdr->CellSpacing();

    FAST int seg = cs->num_segments;
    FAST short *widths = &cs->spacing[0];
    FAST short *ncells = &cs->num_cells[0];

    FAST int maxDist = cs->distToFirst;

    for (FAST int i = 0; i < seg; i++) // Calculate maximum distance & # gates.
      {
          FAST int c = *ncells++;

          FAST int width = *widths++;

          maxDist += (c * width);
    New->DrawTitle(whichRadar,ptr->distance,0.0,0.0,MAX_RECT);

    agc->setMask(0);

    New->DrawTable(A_SET,Max[0],Min[0],param);

    FAST HorizDisplay *New = new HorizDisplay(agc,MAX_RECT,0.0,0.0,nv,0,0);

    FAST u_long *colors = &GeCommand->colorTable[0];
      New->DrawTable(B_SET,Max[1],Min[1],param);
    if (*colors != -1)
      agc->setColorMap((long *)colors,256);

    New->DrawTitle(whichRadar,ptr->distance,0.0,0.0,ptr->size*1000.0);
      New->DrawTable(C_SET,Max[2],Min[2],param);

    New->HashMarks();
    param = ptr->param0;

    New->DrawTable(A_SET,max[0],min[0],param);

      New->DrawTable(B_SET,max[1],min[1],param);

    param = ptr->param2;

    if (nv > 2)
      New->DrawTable(C_SET,max[2],min[2],param);

    agc->setMask(0x80);

    New->DisplaySet(A_SET);

    if (ptr->tmo)
      New->StartTimer(ptr->tmo);

    return(New);
}
