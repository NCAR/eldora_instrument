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
 * Remove a bunch of test code. Added code to handle real data & data
 * scaling.
 *
 * Revision 1.10  1991/11/01  20:03:32  thor
 * Added support for updating time, reading from pipe and scaling max/min.
 *
 * Revision 1.9  1991/10/30  17:57:23  thor
 * Added deletion of converter, cleaned up title drawing code.
 *
 * Revision 1.8  1991/10/24  14:03:27  thor
 * Fixed incorrect max/min values for B/C_SET.
 *
 * Revision 1.7  1991/10/23  20:39:23  thor
 * Added Mouse support.
 *
 * Revision 1.6  1991/10/22  18:03:35  thor
 * Added color table handling.
 *
 * Revision 1.5  1991/10/22  17:19:47  thor
 * Finally updates correctly.
 *
 * Revision 1.4  1991/10/17  16:50:00  thor
 * Major revision - now actually loops correctly (even if only a test!).
 *
 * Revision 1.3  1991/10/16  20:07:30  thor
 * Fixed set references in makeDisplay.
 *
 * Revision 1.2  1991/10/14  20:33:44  thor
 * Added code to compute number of variables, parameters, etc. and plug them
 * in. Also added code to create/draw display.
 *
 * Revision 1.1  1991/10/14  19:22:18  thor
 * Initial revision
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *
 *
 * description:
 *        
    asm volatile ("fmove%.l %0,fpcr" : : "dmi" (cm));

    return(i);
}


static Radial *makeDisplay(Radial *, GraphicController *);

#include "ColorConverter.hh"
static ColorConverter *conv = NULL;

static int whichRadar = FORWARD_RADIAL;

    FAST int count = 9;

    FAST Radial *display = NULL;

    float lastAngle = 360.0;	// Force update of time.

    FAST int radar;

    static int onetime = 0;

    for (;;)
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask,FLAGS_OR);

	  switch(flag)
	    {
	      case DESTROY_SELF:
	      case (DESTROY_SELF | NEW_DATA_FLAG):
		if (display != NULL)
		  {
		      delete(display);
		      display = NULL;
		  }
		if (conv != NULL)
		  {
		      delete(conv);
		      conv = NULL;
		  }
		continue;
		break;

	      case LOAD_ONLY:
		if (GeCommand->cmd == FORWARD_RADIAL)
		  radar = FORWARD_RADIAL;
		else
		  radar = AFT_RADIAL;

		count = 9;
	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		continue;
		DdpCtrl->Clear();
		if (!pipe.Empty())
		  {
		      pipe.Flush();
		  }
		break;
		count = 9;
	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		continue;
		if (radar == FORWARD_RADIAL)
		  DdpCtrl->Fore();
		else
		  DdpCtrl->Aft();
		display = makeDisplay(display,agc);
		break;
		count = 9;
	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
		continue;
		DdpCtrl->Fore();
		if (!pipe.Empty())
		  pipe.Flush();
		RadialMouse(display);
		continue;
		break;

		break;

	      case AFT_RADIAL:
	      case (AFT_RADIAL | NEW_DATA_FLAG):
		whichRadar = AFT_RADIAL;
		radar = whichRadar;
		display = makeDisplay(display,agc);
		lastAngle = 360.0;
		  pipe.Flush();
		break;
		printf("Got pipe data\n");

		pipe.Flush();
		printf("Pipe full\n");
	    }

		if (++count == 10)
		  {
		      display->UpdateClock(now->hour,now->minute,now->second);
		      count = 0;
		  }

		bcopy((char *)&(dataBeam->ray.hour),(char *)now,
				sizeof(short) * 4);

		bcopy((char *)now,(char *)LastTime,
		      sizeof(Beam_Time));
		  continue;
		FAST short *data = (short *)(dataBeam + 1);

		FAST unsigned short *data = (unsigned short *)(dataBeam + 1);
		conv->GetBeam(data,radData);
		  {
		//	  display->drawBeam(radData);
						      // next sweep.
			display->UpdateClock(now->hour,now->minute,
		  self.SetFlags(NEW_DATA_FLAG);
		      if (lastAngle < radData.angle)

	  printf("Looping\n");	// Strictly for testing!!!!!
	  taskDelay(10);

	  self.SetFlags(NEW_DATA_FLAG);	// Strictly for testing!!!!!
			display->UpdateClock(now->hour,now->minute,
					     now->second);
		  }

		lastAngle = radData.angle;

		// Color convert it - only on uniprocessor!
		conv->GetBeam(data,&radData.colors[0]);

		// Draw it.
		display->drawBeam(radData);

		if (pipe.Empty() == FALSE)
		  {
		      self.SetFlags(NEW_DATA_FLAG);
		      continue;
      }
}

static Radial *makeDisplay(FAST Radial *old, FAST GraphicController *agc)
{
    FAST DispCommand *ptr = GeCommand;

    if (old != NULL)
      delete(old);   

    // The following is only for uniprocessor systems!!!!!
    if (conv != NULL)
      delete(conv);

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
		      max[0] = (max[0] - p->parameter_bias) / 
			p->parameter_scale;
		      min[0] = (min[0] - p->parameter_bias) / 
			p->parameter_scale;

    FAST int param = ptr->param0;

    if (param != NO_PARAM)
      {
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
		      max[1] = (max[1] - p->parameter_bias) / 
			p->parameter_scale;
		      min[1] = (min[1] - p->parameter_bias) / 
			p->parameter_scale;

    param = ptr->param1;

    if (nv > 1 && param != NO_PARAM)
      {
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
		      max[2] = (max[2] - p->parameter_bias) / 
			p->parameter_scale;
		      min[2] = (min[2] - p->parameter_bias) / 
			p->parameter_scale;

    param = ptr->param2;

    if (nv > 2 && param != NO_PARAM)
      {
	  FAST char *ptr = ParamTapeNames[ParamToNum(param)];
    conv = new ColorConverter(DISPLAYED_GATES,max,min,offsets,np,nv);
	  FAST int len = strlen(ptr);

	  for (FAST int i = 0; i < np; i++)
    conv->SetBeamSize(*cs);
		PARAMETER *p = Hdr->Parameter(i);

		if (!strncmp(ptr,p->parameter_name,len))
		  {
		      offsets[2] = i;
		      scales[2] = p->parameter_scale;
		      biases[2] = p->parameter_bias;
		      break;
		  }
	    }
      }

    // Again uniprocessor only!
    conv = new ColorConverter(31,max,min,scales,biases,offsets,np,nv);

    CELLSPACING *cs = Hdr->CellSpacing();

    conv->SetBeamSize(*cs,ptr->radius);

    FAST int seg = cs->num_segments;

    FAST int maxDist = cs->distToFirst;
    if (*colors != -1)
      agc->setColorMap((long *)colors,256);

    FAST Radial *New = new Radial(agc,DISPLAYED_GATES,nv,0,0);
      {
          FAST int c = *ncells++;

          FAST int width = *widths++;

          maxDist += (c * width);
      }


	  New->drawTable(A_SET,max[0],min[0],param);

    agc->clear();

    FAST Radial *New = new Radial(agc,ptr->radius,nv,0,0);

    FAST u_long *colors = &GeCommand->colorTable[0];
	  New->drawTable(B_SET,max[1],min[1],param);
    if (*colors != 0xffffffff)
	  agc->setColorMap((long *)colors,256);

    New->SetBounds((float)maxDist,(float)cs->distToFirst);

    FAST int wdw = 0;
	  New->drawTable(C_SET,max[2],min[2],param);
    if (nv)
      {
	  param = ptr->param0;
	  New->drawTable(A_SET,max[0],min[0],param);
	  New->drawTitle(A_SET,whichRadar);
      }

    if (nv > 1)
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
