//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Vert.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#pragma implementation

#include "Vert.hh"

const double Vert::METERS_PER_DEGREE = 112251.29;
const double Vert::TILT_ANGLE = 18.0;

Vert::Vert(GraphicController *gbd) : Display(gbd)
{

    FAST unsigned short xoff = 0;
    FAST unsigned short yoff = 0;

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

    setPriority(0);
}

void Vert::reset(FAST Header *hdr, FAST DispCommand *cmd)
{
    setZoom(ZOOM1);             // Reset zoom and window location.
    curZoom = ZOOM1;
    home();
    lastIndex = 1000;           // So we draw correctly.

    float max[3];               // For the converter.
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

    converter->SetBeamSize(*cs,cmd->radius);

    makeCellVector(*cs);
    
    agc->setMask(0);

    agc->clear();

    FAST u_long *colors = &cmd->colorTable[0];

    if (*colors != 0xffffffff)
          agc->setColorMap((long *)colors,256);

    numOfParams = nv;

    if (cmd->cmd == Display::VERT_AFT)
      dispType = Display::VERT_AFT;
    else
      dispType = Display::VERT_FORE;
    
    numOfWdws = nv * 2;
    altitude = cmd->top;
    distance = cmd->distance;
    whichSide = cmd->side;
    pixelsPerMeter = (double)Display::FULL_WIDTH / (altitude - cmd->bottom);

    // This next bit is gross, but efficient. We really want the
    // tangent of the angle to divide by the distance, but
    // multiplication is faster so we invert it here.
    beamWidth = 1.0 / tan(rd->horz_beam_width);
    
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

    calcX(Vert::TILT_ANGLE);
    
    agc->setMask(0x80);

    setPriority(1);

    displaySet(Display::A_SET);
}

void Vert::displaySet(int set)
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

void Vert::drawTitle(int set, int radar)
{
    FAST int wdw;

    if (set == Display::A_SET)
      wdw = 0;
    else if (set == Display::B_SET)
      wdw = 2;
    else
      wdw = 4;

    setTextScale(wdw,2,2);
    
    FAST char *title;

    if (radar == Display::VERT_FORE)
      title = "Forward Radar";
    else
      title = "Aft Radar";

    Point a;

    a.x = 20;
    a.y = 20;

    horText(wdw,a,title,WHITE);

    // Print # meters per hash mark.
    double meters = (double)Display::FULL_HEIGHT / pixelsPerMeter;
    
    FAST char *metersText = "10 km/div";

    if (meters < 20000.0)
      metersText = "5 km/div";
    else if (meters >= 100000.0)
      metersText = "20 km/div";

    a.x = 500;
    a.y = 20;
    
    horText(wdw,a,metersText,WHITE1);

    hashMarks(meters);
}

void Vert::makeCellVector(CELLSPACING &spacing)
{
    FAST int seg = spacing.num_segments;
    FAST short *widths = &spacing.spacing[0];
    FAST short *ncells = &spacing.num_cells[0];
    FAST int maxDist = spacing.distToFirst;
    FAST int totalCells = 0;
    
    for (FAST int i = 0; i < seg; i++) // Calculate maximum distance & # gates.
      {
          FAST int c = *ncells++;

	  totalCells += c;
	  
          FAST int width = *widths++;

          maxDist += (c * width);
      }

    maxDistance = (double)maxDist;

    cellVector = new int[totalCells];

    widths = &spacing.spacing[0];
    ncells = &spacing.num_cells[0];
    
    FAST int dist = spacing.distToFirst;
    FAST int *cv = cellVector;

    for (i = 0; i < seg; i++)
      {
	  FAST int count = *ncells++;

	  FAST int inc = *widths++;

	  do
	    {
		*cv++ = dist;

		dist += inc;
	    } while (--count);
      }
}

void Vert::hashMarks(double meters)
{
    Point a;
    Point b;
    Point c;
    Point d;

    a.y = 0;
    b.y = Display::FULL_HEIGHT - 1;
    d.x = Display::FULL_WIDTH - 1;
    c.x = 0;

    meters /= 1000.0;           // Convert to km.
    float ppkm = (float)Display::FULL_WIDTH / meters; // Pixels/meter.

    FAST int l = (int)(ppkm + .5);

    if (meters >= 100.0)
      l *= 20;                  // Draw every 20 km.
    if (meters >= 20.0)
      l *= 10;                  // Draw every 10 km.
    else
      l *= 5;                   // Draw every 5 km.

    FAST int j = Display::FULL_WIDTH;

    FAST int nparams = numOfParams;

    FAST Window *wdw = Wdw;
    for (FAST int m = 0; m < nparams; m++, wdw += 2)
      {
          for (FAST int k = l; k < j; k += l)
            {
                a.x = k;
                b.x = k;
                c.y = k;
                d.y = k;
          
                wdw->line(a,b,WHITE);
                wdw->line(c,d,WHITE);
            }
      }
}

void Vert::calcX(double tilt)
{
    // Calculate the best x location for the aircraft.
    if (dispType == Display::VERT_AFT)
      {
          if (whichSide == Vert::LEFT)
            aircraft_x = Display::FULL_WIDTH - 1;
          else
            aircraft_x = 0;
      }
    else
      {
          double maxd = maxDistance;

          double tiltinc = (90.0 - tilt) / 90.0;

          double dist = distance;

          double x;

          for (double elev = 0.5; elev < 90.0; elev += 0.5)
            {
                double theta = tilt + (tiltinc * elev);

                double z = sin(DegreesToRadians(elev)) * maxd;
                double y = cos(DegreesToRadians(theta)) * z;

                if (y < dist)
                  break;
                else
                  x = sin(DegreesToRadians(theta)) * z;
            }
          int ix = fastround(x * pixelsPerMeter);

          if (whichSide == Vert::LEFT)
            aircraft_x = Display::FULL_WIDTH - ix;
          else
            aircraft_x = ix;
      }
}

Vert::~Vert(void)
{
}
