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
		      display = makeDisplay(display,agc);
		      break;

		    case FORWARD_RADIAL:
		      whichRadar = FORWARD_RADIAL;
		      display = makeDisplay(display,agc);
		      break;

		    case AFT_RADIAL:
		      whichRadar = AFT_RADIAL;
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
//	  conv->GetBeam(theData,radData);
	  // Draw it.
//	  display->drawBeam(radData);
	  flag = self.QueryFlags();
			display->UpdateClock(now->hour,now->minute,
					     now->second);
		  }

		lastAngle = radData.angle;

		// Color convert it - only on uniprocessor!
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

    FAST int param = ptr->param0;

    if (param != NO_PARAM)
      {
	  FAST char *ptr = ParamTapeNames[ParamToNum(param)];

    if (param != NO_PARAM)

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

    param = ptr->param1;

    if (nv > 1 && param != NO_PARAM)
      {
	  FAST char *ptr = ParamTapeNames[ParamToNum(param)];

    if (param != NO_PARAM)

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

    param = ptr->param2;

    if (nv > 2 && param != NO_PARAM)
      {
	  FAST char *ptr = ParamTapeNames[ParamToNum(param)];
    conv = new ColorConverter(DISPLAYED_GATES,max,min,offsets,np,nv);
	  FAST int len = strlen(ptr);

	  for (FAST int i = 0; i < np; i++)
    conv->SetBeamSize(*cs);
			      
    if (old != NULL)
      delete(old);

    FAST Radial *New = new Radial(agc,DISPLAYED_GATES,ptr->numParams,0,0);
      {
    param = ptr->param0;

    if (param != NO_PARAM)
      {
	  New->drawTable(A_SET,max[0],min[0],param);

    agc->clear();

    param = ptr->param1;

    if (param != NO_PARAM)

	  New->drawTable(B_SET,max[0],min[0],param);
	  New->drawTitle(A_SET,whichRadar);
	  agc->setColorMap((long *)colors,256);

    param = ptr->param2;

    if (param != NO_PARAM)

	  New->drawTable(C_SET,max[0],min[0],param);
	  New->drawTitle(A_SET,whichRadar);
      {
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
