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
 *
 * Revision 1.1  1991/10/28  19:37:49  thor
 *
 *
 *
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "GeDraw.hh"

static HorizDisplay *makeDisplay(HorizDisplay *, GraphicController *);

#include "ColorConverter.hh"
static ColorConverter *conv = NULL;
static Horiz *horizFilter = NULL;
void HorizLoop(FAST Task &self, FAST GraphicController *agc)
static int whichRadar = FORWARD_HORIZ;

void HorizLoop(FAST Task &self, FAST GraphicController *agc, FAST Pipe &pipe)
    HorizDisplay *display = NULL;
    self.FlagsInit();
    int args;
    FAST HorizDisplay *display = NULL;

    Pipe dataPipe(sizeof(HorizPoint),100);
	  FAST unsigned int flag = self.WaitOnFlags(waitMask | NEW_DATA_FLAG |
						    DESTROY_SELF | MOUSE_FLAG,
      {

	  FAST unsigned int flag = self.WaitOnFlags(waitMask | TMO_FLAG,
						    FLAGS_OR);

	  switch(flag)
	    {
		  delete(display);
		      delete(horizFilter);
		      horizFilter = NULL;
		  }
		continue;
	      case STOP | NEW_DATA_FLAG:

	      case LOAD_ONLY:
		if (GeCommand->cmd == FORWARD_RADIAL)
		  radar = FORWARD_RADIAL;
		else
		  radar = AFT_RADIAL;

	      case START | NEW_DATA_FLAG:
		continue;
		break;
	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		continue;
		continue;
		break;

	      case START:
	      case RELOAD:
	      case (RELOAD | NEW_DATA_FLAG):
	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		continue;
		reset = 1;
                if (!pipe.Empty())
		  pipe.Flush();
		break;

	      case (FORWARD_HORIZ | NEW_DATA_FLAG):
	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		continue;
		break;

	      case AFT_HORIZ:
		HorizMouse(display);
		continue;
		break;

	      case (AFT_HORIZ | NEW_DATA_FLAG):
		whichRadar = AFT_HORIZ;
                radar = whichRadar;
		break;
		{
		      m.latitude = dataBeam->air.latitude;
	  // Color convert it - only on uniprocessor!
		      display->Shift(&m);
	  // Draw it.
//	  display->drawBeam(radData);

	  printf("Looping\n");	// Strictly for testing!!!!!
	  taskDelay(10);

	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		// Draw it.
		while (dataPipe.Empty() == FALSE)
		  {
		      HorizPoint dp;

		      dataPipe.Read(&dp);

		      display->Draw(&dp);
		  }

		if (pipe.Empty() == FALSE)
		  self.SetFlags(NEW_DATA_FLAG);
    if (conv != NULL)
      }
	  delete(conv);
}

static HorizDisplay *makeDisplay(FAST HorizDisplay *old,
				 FAST GraphicController *agc)
{
    if (old != NULL)
      delete(old);   

    // The following is only for uniprocessor systems!!!!!
    if (horizFilter != NULL)
      {
	  delete(horizFilter);
      }

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

          FAST int c = *ncells++;

          FAST int width = *widths++;


    FAST HorizDisplay *New = new HorizDisplay(agc,MAX_RECT,0.0,0.0,nv,0,0);
          maxDist += (c * width);
    New->DrawTitle(whichRadar,ptr->distance,0.0,0.0,MAX_RECT);

    agc->setMask(0);

    if (param != NO_PARAM)
      New->DrawTable(A_SET,max[0],min[0],param);

    FAST HorizDisplay *New = new HorizDisplay(agc,MAX_RECT,0.0,0.0,nv,0,0);

    if (param != NO_PARAM)
      New->DrawTable(B_SET,max[1],min[1],param);
    if (*colors != -1)
      agc->setColorMap((long *)colors,256);

    if (param != NO_PARAM)
      New->DrawTable(C_SET,max[2],min[2],param);

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
