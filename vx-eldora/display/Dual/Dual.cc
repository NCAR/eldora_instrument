//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Dual.cc
//	Original Author: Richard E. K. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.7  1994/08/12  18:01:36  thor
// Fixed another nit.
//
// Revision 1.6  1994/04/08  20:31:30  thor
// Many changes!
//
// Revision 1.5  1993/10/28  16:57:47  thor
// Changed grid pattern to center on aircraft in x. Fixed Clock calls.
//
// Revision 1.4  1993/10/21  16:38:57  thor
// Temp save of latest work.
//
// Revision 1.3  1993/09/03  17:10:25  thor
// Fixed consts to avoid clashing with others.
//
// Revision 1.3  1993/09/03  17:10:25  thor
// Fixed consts to avoid clashing with others.
//
// Revision 1.2  1993/08/31  16:39:41  thor
// The first really working version.
//
// Revision 1.1  1992/11/06  20:20:20  thor
// Initial revision
//
//
//
// description:
//        
//
//
#pragma implementation

#include "Dual.hh"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Dual::Dual(GraphicController *gbd) : Display(gbd)
{
    unsigned short xoff = 0;
    unsigned short yoff = 0;
    
    Wdw[0].init(gbd,0,0,0,Display::FULL_WIDTH,(Display::FULL_HEIGHT / 2),
		0,0);
    Wdw[2].init(gbd,1,0,(Display::FULL_HEIGHT / 2),Display::FULL_WIDTH,
		(Display::FULL_HEIGHT / 2),Display::FULL_WIDTH,0);
    Wdw[1].init(gbd,2,Display::FULL_WIDTH,0,Display::TBL_WIDTH,
		Display::FULL_HEIGHT,0,Display::FULL_HEIGHT);
    
    Wdw[0].setPriority(0);
    Wdw[1].setPriority(0);
    Wdw[2].setPriority(0);

    // Set the base video memory addresses.
    videoMemory[0] = (unsigned char *)gbd->videoBufferAddr();
    videoMemory[1] = videoMemory[0] + Display::FULL_WIDTH;

    numOfWdws = 3;
}

void Dual::reset(FAST Header *hdr, FAST DispCommand *cmd)
{
    setZoom(ZOOM1);             // Reset zoom and window location.
    curZoom = ZOOM1;
    home();
    lastIndex = 1000;           // So we draw correctly.

    float max[2];               // For the converter.
    float min[2];
    float scales[2];
    float biases[2];

    max[0] = cmd->max0;
    max[1] = cmd->max1;

    min[0] = cmd->min0;
    min[1] = cmd->min1;

    FAST int nv = 2; // How many parameters are active? 2 always!

    FAST RADARDESC *rd = hdr->Radar(1);

    FAST int np = rd->num_parameter_des; // How are there total?

    int offsets[2];

    FAST int param = cmd->param0;
    
    FAST const char *ptr = namer->paramToName(param);

    FAST int len = strlen(ptr);

    for (FAST int i = 0; i < np; i++)
      {
	  PARAMETER *p = hdr->Parameter(i);

	  if (!strncmp(ptr,p->parameter_name,len))
	    {
		offsets[0] = i;
		scales[0] = p->parameter_scale;
		biases[0] = p->parameter_bias;

		if (param == ParamNames::VELOCITY)
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

    param = cmd->param1;

    ptr = namer->paramToName(param);

    len = strlen(ptr);

    for (i = 0; i < np; i++)
      {
	  PARAMETER *p = hdr->Parameter(i);

	  if (!strncmp(ptr,p->parameter_name,len))
	    {
		offsets[1] = i;
		scales[1] = p->parameter_scale;
		biases[1] = p->parameter_bias;

		if (param == ParamNames::VELOCITY)
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

    converter->Reset(31,max,min,scales,biases,offsets,np,nv);

    CELLSPACING *cs = hdr->CellSpacing();

    float maxalt = cmd->top;
    float minalt = cmd->bottom;
    
    maxAlt = maxalt;
    minAlt = minalt;

    float ppm = (float)(Display::FULL_HEIGHT / 2) / (maxalt - minalt);

    pixelsPerMeter = ppm;

    float f = ppm * cs->distToFirst;

    firstGate = fastround(f);

    radius = Display::FULL_HEIGHT;

    converter->SetBeamSize(*cs,radius,(1.0/ppm));

    agc->setMask(0);

    agc->clear();

    if (cmd->userColors)
          setColors();

    numOfParams = nv;

    if (cmd->cmd == AFT_DUAL)
      dispType = Display::DUAL_AFT;
    else
      dispType = Display::DUAL_FORE;
    
    param = cmd->param0;

    drawTable(Display::A_SET,max[0],min[0],param);

    param = cmd->param1;
    
    drawTable(Display::B_SET,max[1],min[1],param);

    drawTitle(Display::A_SET,dispType);

    refreshClock();

    Point cpt;

    cpt.x = 0;
    cpt.y = 0;

    moveClock(cpt);

    agc->setMask(0x80);

    setPriority(1);

    displaySet(Display::A_SET);
}

void Dual::undisplay(void)
{
    FAST Window *ptr = Wdw;

    ptr->undisplay();
    ptr++;
    ptr->undisplay();
    ptr++;
    ptr->undisplay();
}

void Dual::drawTable(int set, float max, float min, FAST int param, 
			FAST int tblsize)
{
    FAST Window *wdw = &Wdw[1];;
    FAST int offset;
    FAST int top;
    FAST int base;

    if (set == A_SET)
      {
	  top = 0;
	  base = (Display::FULL_HEIGHT / 2) - 1;
	  offset = tblsize;
      }
    else if (set == B_SET)
      {
	  top = (Display::FULL_HEIGHT / 2);
	  base = Display::FULL_HEIGHT;
	  offset = tblsize * 2;	// Bias into color lut.
      }
    else
      return;

    wdw->setTextScale(2,2);
    wdw->setTextBackGround(BLACK);
    
    FAST char *title = "Unknown";

    FAST int index = namer->paramToNum(param);

    if (index != -1)
      title = namer->numToName(index);

    Point a;
    Point b;

    a.x = 15;
    a.y = ((Display::FULL_HEIGHT / 2) / 2) + top;

    wdw->horText(a,title,WHITE);
    
    a.x = Display::TBL_WIDTH - 87;
    a.y = top + 15;

    b.x = Display::TBL_WIDTH - 31;;
    b.y = top + 15;

    --tblsize;			// Both must decremented, since we
				// start at zero.
    --offset;

    double step = (double)(max - min) / (double)tblsize;
    double m = (double)max;

    for (tblsize++; tblsize > 0; --tblsize, --offset)
      {
	  char label[10];

	  wdw->frect(b,16,16,offset);

	  switch(tblsize)	// This is predicated on 31 colors!!!!!
	    {
	      case 31:
	      case 25:
	      case 20:
	      case 16:
	      case 10:
	      case 5:
	      case 1:
		int i = iround(m);
		sprintf(label,"%d",i);
		wdw->horText(a,label,WHITE);
		break;
	    }
          m -= step;
          a.y += 16;
          b.y += 16;
      }
}

void Dual::drawTitle(int set, int radar)
{
    FAST Window *wdw1 = &Wdw[0];
    FAST Window *wdw2 = &Wdw[2];
    FAST Window *wdw3 = &Wdw[1];;

    wdw3->setTextScale(2,2);
    wdw3->setTextBackGround(BLACK);

    Point a;
    Point b;

    a.x = 14;
    a.y = 20;

    float ppm = pixelsPerMeter;
    float max = maxAlt;
    float min = minAlt;

    max /= 1000.0;
    min /= 1000.0;

    char label[20];

    sprintf(label,"%4.1f",max);

    strcat(label," km"); 

    wdw3->horText(a,label,WHITE);

    a.y += (Display::FULL_HEIGHT / 2);

    wdw3->horText(a,label,WHITE);

    a.y = (Display::FULL_HEIGHT / 2) - 15;

    sprintf(label,"%4.1f",min);

   strcat(label," km");

    wdw3->horText(a,label,WHITE);

    a.y += (Display::FULL_HEIGHT / 2);

    wdw3->horText(a,label,WHITE);

    float dist = max - min;

    FAST short ppkm = (short)iround((ppm * 1000.0) + .5);

    FAST char *div = "10 km/div";

    if (dist <= 10.0)
      {
	  ppkm *= 5;
	  div = "5 km/div";
      }
    else if (dist <= 100.0)
      ppkm *= 10;
    else
      {
	  div = "20 km/div";
	  ppkm *= 20;
      }

    a.x = 14;
    a.y = Display::FULL_HEIGHT - 40;

    wdw3->horText(a,div,WHITE);

    a.y -= (Display::FULL_HEIGHT / 2);

    wdw3->horText(a,div,WHITE);

    // Draw the grid lines. The vertical lines are centered on the
    // aircraft's postion.
    a.x = Display::FULL_WIDTH / 2;
    a.y = 0;
    b.x = Display::FULL_WIDTH / 2;
    b.y = (Display::FULL_HEIGHT / 2) - 1;

    FAST short limit = Display::FULL_WIDTH - 1;
    
    while (a.x < limit)
      {
	  wdw1->line(a,b,WHITE);
	  wdw2->line(a,b,WHITE);
	  a.x += ppkm;
	  b.x += ppkm;
      }

    a.x = (Display::FULL_WIDTH / 2) - ppkm;
    b.x = (Display::FULL_WIDTH / 2) - ppkm;

    while (a.x < 0x7fff)	// God, I wish I'd used sign shorts!
      {
	  wdw1->line(a,b,WHITE);
	  wdw2->line(a,b,WHITE);
	  a.x -= ppkm;
	  b.x -= ppkm;
      }

    limit = (Display::FULL_HEIGHT / 2) - 1;

    a.x = 0;
    a.y = ppkm;
    b.x = Display::FULL_WIDTH - 1;
    b.y = ppkm;

    while (a.y < limit)
      {
	  wdw1->line(a,b,WHITE);
	  wdw2->line(a,b,WHITE);
	  a.y += ppkm;
	  b.y += ppkm;
      }

    a.x = 0;
    a.y = 0;
    b.x = Display::FULL_WIDTH - 1;
    b.y = 0;

    wdw2->line(a,b,WHITE);

    a.y = (Display::FULL_HEIGHT / 2) - 1;
    b.y = (Display::FULL_HEIGHT / 2) - 1;

    wdw1->line(a,b,WHITE);

    a.y = (Display::FULL_HEIGHT / 2) - 1;
    b.x = Display::TBL_WIDTH - 1;
    b.y = (Display::FULL_HEIGHT / 2) - 1;

    wdw3->line(a,b,WHITE);

    a.y++;
    b.y++;

    wdw3->line(a,b,WHITE);
}

void Dual::displaySet(FAST int set)
{
    FAST Window *ptr = Wdw;
    
    ptr->display();
    ptr++;
    ptr->display();
    ptr++;
    ptr->display();
}

void Dual::switchSets(void)
{
}
    
Dual::~Dual(void)
{
}
