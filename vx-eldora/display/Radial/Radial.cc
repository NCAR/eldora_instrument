//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Radial.cc
//	Original Author: Richard E. K. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 2.6  1994/09/19  16:02:03  thor
// Added new color setting & clock stuff.
//
// Revision 2.5  1994/07/06  14:09:32  thor
// The latest & greatest - too many changes to list.
//
// Revision 2.4  1993/11/22  16:58:30  thor
// Added code to support resetting.
//
// Revision 2.3  1993/10/27  14:37:09  thor
// Add ZOOM8 & fixed clk calls.
//
// Revision 2.2  1993/07/29  19:21:37  thor
// Upgraded to VxWorks 5.1.
//
// Revision 2.1  1993/07/01  16:40:28  thor
// Brought up to latest ANSI spec.
//
// Revision 2.0  1992/11/03  12:52:47  thor
// First offical ELDORA release!
//
// Revision 1.17  1992/06/05  13:41:09  thor
// Changed method of generating number labels to make it smoother.
//
// Revision 1.16  1992/06/05  13:08:40  thor
// Fixed centering of number labels.
//
// Revision 1.15  1992/05/04  16:46:58  thor
// Added/changed code to handle display of parameter values hopefully to
// most people's liking.
//
// Revision 1.14  1992/02/27  19:25:12  thor
// Changed manner of getting correct title to use table.
//
// Revision 1.13  1992/02/05  20:44:06  thor
// Faster firstGate calc.
//
// Revision 1.12  1992/02/05  19:39:32  thor
// Changed code to put any radius circle at center of data window.
//
// Revision 1.11  1991/12/03  18:03:14  thor
// Changed hash circles to draw at 5 or 10 km depending on max distance.
//
// Revision 1.10  1991/12/03  17:33:30  thor
// Changed table drawing to use integer steps & less text.
//
// Revision 1.9  1991/11/14  18:20:22  thor
// Fixed incorrect calculation of first gate index.
//
// Revision 1.8  1991/10/24  15:46:42  thor
// Changed nextZoom to eliminate extraneous cursor location diddling.
//
// Revision 1.7  1991/10/22  18:10:08  thor
// Fixed signal/noise ratio text to fit screen. Added NCP (normalized
// coherent poer).
//
// Revision 1.6  1991/10/21  18:08:37  thor
// Fixed to draw text on black background, divide max distance by 1000.
//
// Revision 1.5  1991/10/09  18:03:03  thor
// Fixed name for reflectivity code.
//
// Revision 1.4  1991/06/13  17:52:26  thor
// Added SetBounds method.
// Rewrote drawTitle method to use radar type.
// Added signal to noise ratio text.
//
// Revision 1.3  1991/05/06  15:08:51  thor
// Added embedded Clock class for time display.
//
// Revision 1.2  1991/04/29  18:47:43  thor
// Fixed angles so we match radar conventions.
//
// Revision 1.1  1991/04/08  20:28:42  thor
// Initial revision
//
//
// description:
//        This file contains all but the drawing and inline methods
// for the Radial class.
//
//
#pragma implementation

#include "Radial.hh"

#include <taskLib.h>

Radial::Radial(GraphicController *gbd) : Display(gbd)
{
    FAST unsigned short xoff = 0;
    FAST unsigned short yoff = 0;

    // Set the base video memory addresses.
    videoMemory[0] = (unsigned char *)gbd->videoBufferAddr() + xoff +
      (yoff * 4096);
    videoMemory[1] = videoMemory[0] + Display::FULL_WIDTH;
    videoMemory[2] = videoMemory[0] + (Display::FULL_HEIGHT * 4096);

    // Set up the offsets to each center point.
    FAST int center = (Display::FULL_WIDTH / 2) - 1;
    videoBase[0] = videoMemory[0] + center + (center * 4096);
    videoBase[1] = videoMemory[1] + center + (center * 4096);
    videoBase[2] = videoMemory[2] + center + (center * 4096);

    // The color tables are located directly after the third parameter
    // data area.
    FAST unsigned short tbl_x = xoff + Display::FULL_WIDTH;
    FAST unsigned short tbl_y = yoff + Display::FULL_HEIGHT;

    Wdw[0].init(gbd,0,0,0,Display::FULL_WIDTH,Display::FULL_HEIGHT,xoff,yoff);
    Wdw[1].init(gbd,1,Display::FULL_WIDTH,0,Display::TBL_WIDTH,
		Display::FULL_HEIGHT,tbl_x,tbl_y);

    xoff += Display::FULL_WIDTH;
    tbl_x += Display::TBL_WIDTH;

    Wdw[2].init(gbd,0,0,0,Display::FULL_WIDTH,Display::FULL_HEIGHT,xoff,yoff);
    Wdw[3].init(gbd,1,Display::FULL_WIDTH,0,Display::TBL_WIDTH,
		Display::FULL_HEIGHT,tbl_x,tbl_y);

    xoff -= Display::FULL_WIDTH;
    yoff += Display::FULL_HEIGHT;
    tbl_x += TBL_WIDTH;

    Wdw[4].init(gbd,0,0,0,Display::FULL_WIDTH,Display::FULL_HEIGHT,xoff,yoff);
    Wdw[5].init(gbd,1,Display::FULL_WIDTH,0,Display::TBL_WIDTH,
		Display::FULL_HEIGHT,tbl_x,tbl_y);

    FAST Window *wdw = Wdw;

    setPriority(0);

    makeStr->setf(ios::fixed);
    makeStr->precision(2);
}

void Radial::reset(FAST Header *hdr, FAST DispCommand *cmd)
{
    setZoom(ZOOM1);		// Reset zoom and window location.
    curZoom = ZOOM1;
    home();
    lastIndex = 1000;		// So we draw correctly.

    float max[3];		// For the converter.
    float min[3];
    float scales[3];
    float biases[3];

    max[0] = cmd->max0;
    max[1] = cmd->max1;
    max[2] = cmd->max2;

    min[0] = cmd->min0;
    min[1] = cmd->min1;
    min[2] = cmd->min2;

    FAST int nv = cmd->numParams; // How many parameters are active?

    FAST RADARDESC *rd = hdr->Radar(1);

    FAST int np = rd->num_parameter_des; // How are there total?

    int offsets[3];

    FAST int param = cmd->param0;

    if (param != ParamNames::NO_PARAM)
      {
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
      }

    param = cmd->param1;

    if (nv > 1 && param != ParamNames::NO_PARAM)
      {
          FAST const char *ptr = namer->paramToName(param);

          FAST int len = strlen(ptr);

          for (FAST int i = 0; i < np; i++)
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
      }

    param = cmd->param2;

    if (nv > 2 && param != ParamNames::NO_PARAM)
      {
          FAST const char *ptr = namer->paramToName(param);

          FAST int len = strlen(ptr);

          for (FAST int i = 0; i < np; i++)
            {
                PARAMETER *p = hdr->Parameter(i);

                if (!strncmp(ptr,p->parameter_name,len))
                  {
                      offsets[2] = i;
                      scales[2] = p->parameter_scale;
                      biases[2] = p->parameter_bias;

                      if (param == ParamNames::VELOCITY)
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

    converter->Reset(31,max,min,scales,biases,offsets,np,nv);

    CELLSPACING *cs = hdr->CellSpacing();

    FAST int seg = cs->num_segments;
    FAST short *widths = &cs->spacing[0];
    FAST short *ncells = &cs->num_cells[0];

    FAST int maxDist = cs->distToFirst;

    firstGate = maxDist;
    
    cout << "First gate at " << firstGate << endl;
    for (FAST int i = 0; i < seg; i++) // Calculate maximum distance & # gates.
      {
          FAST int c = *ncells++;

          FAST int width = *widths++;

          maxDist += (c * width);
      }

    maxDistance = (float)maxDist;

    double ppm = (double)Display::FULL_HEIGHT / (double)maxDist;

    firstGate = fastround((double)cs->distToFirst * ppm);
    
    converter->SetBeamSize(*cs,cmd->radius);
    
    agc->setMask(0);

    agc->clear();

    if (cmd->userColors)
          setColors();

    numOfParams = nv;
    radius = cmd->radius;
    if (cmd->cmd == AFT_RADIAL)
      dispType = Display::RADIAL_AFT;
    else
      dispType = Display::RADIAL_FORE;
    
    numOfWdws = nv * 2;
    
    param = cmd->param0;
    drawTable(Display::A_SET,max[0],min[0],param);
    drawTitle(Display::A_SET,dispType);
    
    if (nv > 1)
      {
          param = cmd->param1;
          drawTable(Display::B_SET,max[1],min[1],param);
          drawTitle(Display::B_SET,dispType);

	  if (nv > 2)
	    {
		param = cmd->param2;
		drawTable(Display::C_SET,max[2],min[2],param);
		drawTitle(Display::C_SET,dispType);
	    }
      }

    refreshClock();

    Point cpt;

    cpt.x = Display::FULL_WIDTH + Display::TBL_WIDTH - Clock::WIDTH;
    cpt.y = Display::FULL_HEIGHT - Clock::HEIGHT;

    moveClock(cpt);
    
    agc->setMask(0x80);

    setPriority(1);

    displaySet(Display::A_SET);
}
    
void Radial::drawTitle(FAST int set, FAST int radar)
{
    FAST int wdw;

    if (set == Display::A_SET)
      wdw = 0;
    else if (set == Display::B_SET)
      wdw = 2;
    else
      wdw = 4;

    Point a;

    a.x = 15;
    a.y = 20;

    setTextScale(wdw,2,2);
    
    FAST char *title;

    if (radar == Display::RADIAL_FORE)
      title = "Forward Radar";
    else
      title = "Aft Radar";

    horText(wdw,a,title,WHITE);

    FAST int rad = radius;
    FAST int center = (Display::FULL_WIDTH / 2) - 1;

    a.x = center;
    a.y = center;

    // Draw hash circles.
    
    float max = maxDistance * cos(BEAM_OFFSET);

    max /= 1000.0;

    FAST int ppkm = (int)(((float)rad / max) + .5);

    FAST char *label = "10 km/div";

    if (max >= 20.0)		// If less then 20 km, draw hash marks
				// at 5km, otherwise at 10 km.
      ppkm *= 10;
    else
      {
	  ppkm *= 5;
	  label = "5 km/div";
      }

    FAST int i = ppkm;

    while (i < rad)
      {
	  circle(wdw,a,i,WHITE);
	  taskDelay(1);
	  i += ppkm;
      }

    circle(wdw,a,rad,WHITE);

    Point b;

    // Draw cross hairs.
    a.x = center - rad;;

    b.x =  center + rad;
    b.y = center;

    line(wdw,a,b,WHITE);

    a.x = center;
    a.y = center - rad;
    b.x = center;
    b.y = center + rad;

    line(wdw,a,b,WHITE);

    a.x = 10;
    a.y = 975;

    horText(wdw,a,label,WHITE);

    a.x = 770;

    resetString();

    *makeStr << max << " km";

    horText(wdw,a,outputStr,WHITE);
}

void Radial::displaySet(FAST int set)
{
    FAST int np = numOfParams;

    if (set == Display::A_SET)
      {
          Wdw[0].display();
          Wdw[1].display();
          displayedSet = Display::A_SET;
      }
    else if ((set == B_SET) && (np > 1))
      {
          Wdw[2].display();
          Wdw[3].display();
          displayedSet = Display::B_SET;
      }
    else if (np > 2)
      {
          Wdw[4].display();
          Wdw[5].display();
          displayedSet = Display::C_SET;
      }
}


Radial::~Radial(void)
{
}
