//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Horiz.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.3  1994/09/23  19:50:36  thor
// Changed formatted string output to use strstreams.
//
// Revision 1.2  1994/09/23  15:00:02  thor
// New color code, moved windows about in AGC memory to fit in clock,
// added clock code, fixed meters.
//
// Revision 1.1  1994/04/08  20:29:50  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#pragma implementation

#include "Horiz.hh"

Horiz::Horiz(GraphicController *gbd) : Display(gbd)
{
    FAST unsigned short xoff = 0;
    FAST unsigned short yoff = Horiz::TITLE_HEIGHT;

    // The color tables are located directly after the third parameter
    // data area.
    FAST unsigned short tbl_x = Horiz::DATA_WIDTH;
    FAST unsigned short tbl_y = 0;

    FAST unsigned short title_x = 0;
    FAST unsigned short title_y = 0;

    Wdw[0].init(gbd,0,0,Horiz::TITLE_HEIGHT,Horiz::DATA_WIDTH,
		Horiz::DATA_HEIGHT,xoff,yoff);
    Wdw[1].init(gbd,1,0,0,Horiz::TITLE_WIDTH,Horiz::TITLE_HEIGHT,title_x,
		title_y);
    Wdw[6].init(gbd,2,Horiz::DATA_WIDTH,0,Horiz::TBL_WIDTH,Horiz::TBL_HEIGHT,
		tbl_x,tbl_y);

    yoff += Horiz::DATA_HEIGHT + (2 * Horiz::TITLE_HEIGHT);
    title_y += Horiz::DATA_HEIGHT + Horiz::TITLE_HEIGHT;
    tbl_x += Horiz::TBL_WIDTH;

    Wdw[2].init(gbd,0,0,Horiz::TITLE_HEIGHT,Horiz::DATA_WIDTH,
		Horiz::DATA_HEIGHT,xoff,yoff);
    Wdw[3].init(gbd,1,0,0,Horiz::TITLE_WIDTH,Horiz::TITLE_HEIGHT,title_x,
		title_y);
    Wdw[7].init(gbd,2,Horiz::DATA_WIDTH,0,Horiz::TBL_WIDTH,Horiz::TBL_HEIGHT,
		tbl_x,tbl_y);

    xoff += Horiz::DATA_WIDTH;
    title_x += Horiz::TITLE_WIDTH;
    tbl_x += Horiz::TBL_WIDTH;

    Wdw[4].init(gbd,0,0,Horiz::TITLE_HEIGHT,Horiz::DATA_WIDTH,
		Horiz::DATA_HEIGHT,xoff,yoff);
    Wdw[5].init(gbd,1,0,0,Horiz::TITLE_WIDTH,Horiz::TITLE_HEIGHT,title_x,
		title_y);
    Wdw[8].init(gbd,2,Horiz::DATA_WIDTH,0,Horiz::TBL_WIDTH,Horiz::TBL_HEIGHT,
		tbl_x,tbl_y);

    FAST Window *wdw = Wdw;

    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0); wdw++;
    wdw->setPriority(0);

    Wdw[1].setTextBackGround(BLACK);
    Wdw[3].setTextBackGround(BLACK);
    Wdw[5].setTextBackGround(BLACK);

    double d = 0.0;
    
    for (FAST int i = 0; i < 180; i++, d += 0.5)
        {
            isines[i] = 1.0 / sin(DegreesToRadians(d));
            itangents[i] = 1.0 / tan(DegreesToRadians(d));
        }
    
    makeStr->setf(ios::fixed);
    makeStr->precision(2);
}

void Horiz::reset(FAST Header *hdr, FAST DispCommand *cmd)
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

    if (cmd->userColors)
          setColors();

    numOfParams = nv;

    if (cmd->cmd == Display::HORIZ_AFT)
      dispType = Display::HORIZ_AFT;
    else
      dispType = Display::HORIZ_FORE;
    
    numOfWdws = nv * 3;
    altitude = cmd->distance;
    
    pixelsPerMeter = (double)Horiz::DATA_WIDTH / cmd->size;

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

    refreshClock();

    Point cpt;

    cpt.x = Display::FULL_WIDTH + Display::TBL_WIDTH - Clock::WIDTH;
    cpt.y = Display::FULL_HEIGHT - Clock::HEIGHT;

    moveClock(cpt);
    
    agc->setMask(0x80);

    setPriority(1);

    displaySet(Display::A_SET);

    latitude[0] = (double)Horiz::IMPOSSIBLE;
}

void Horiz::displaySet(int set)
{
    FAST int np = numOfParams;

    if (set == Display::A_SET)
      {
          Wdw[0].display();
          Wdw[1].display();
	  Wdw[6].display();
          displayedSet = Display::A_SET;
      }
    else if ((set == B_SET) && (np > 1))
      {
          Wdw[2].display();
          Wdw[3].display();
	  Wdw[7].display();
          displayedSet = Display::B_SET;
      }
    else if (np > 2)
      {
          Wdw[4].display();
          Wdw[5].display();
	  Wdw[8].display();
          displayedSet = Display::C_SET;
      }
}

void Horiz::switchSets(void)
{
    FAST int nparams = numOfParams;
    FAST int dset = displayedSet;

    if (dset == Display::A_SET)
      {
          if (nparams > 1)
            {
                dset = Display::B_SET;
                
                Wdw[0].undisplay();
                Wdw[1].undisplay();
                Wdw[6].undisplay();
                
                Wdw[2].display();
                Wdw[3].display();
                Wdw[7].display();
            }
      }
    else if (dset == Display::B_SET)
      {
          Wdw[2].undisplay();
          Wdw[3].undisplay();
          Wdw[7].undisplay();
          
          if (nparams == 2)
            {
                dset = Display::A_SET;

                Wdw[0].display();
                Wdw[1].display();
                Wdw[6].display();
            }
          else
            {
                dset = Display::C_SET;

                Wdw[4].display();
                Wdw[5].display();
                Wdw[8].display();
            }
      }
    else
      {
          dset = Display::A_SET;
          Wdw[4].undisplay();
          Wdw[5].undisplay();
          Wdw[8].undisplay();
          
          Wdw[0].display();
          Wdw[1].display();
          Wdw[6].display();
      }
    displayedSet = dset;
}

void Horiz::drawTitle(int set, int radar)
{
    FAST int wdw;

    if (set == Display::A_SET)
      wdw = 1;
    else if (set == Display::B_SET)
      wdw = 3;
    else
      wdw = 5;

    setTextScale(wdw,2,2);

    resetString();

    if (radar == Display::HORIZ_FORE)
      *makeStr << "Forward Radar";
    else
      *makeStr << "Aft Radar";

    // Print # meters per hash mark.
    double meters = (double)Horiz::DATA_WIDTH / pixelsPerMeter;
    
    if (meters >= 500000.0)
      *makeStr << "  100 km/div";
    else if (meters >= 100000.0)
      *makeStr << "  20 km/div";
    else if (meters < 20000.0)
      *makeStr << "  5 km/div";
    else
      *makeStr << "  10 km/div";

    double d = altitude / 1000.0;
    
    *makeStr << "  " << d << " km elevation";

    Point a;

    a.x = 20;
    a.y = 20;

    horText(wdw,a,outputStr,WHITE1);

    hashMarks(meters);
    degrees = meters / METERS_PER_DEGREE;
    eighth =  degrees / 8.0;
}

void Horiz::drawTable(int set, float max, float min, int param, int tblsize)
{
    FAST int wdw;
    FAST int offset;            // Starting color lut index.

    if (set == Display::A_SET)
      {
          wdw = 6;
          offset = tblsize;
      }
    else if (set == Display::B_SET)
      {
          wdw = 7;
          offset = 2 * tblsize;
      }
    else
      {
          wdw = 8;
          offset = 3 * tblsize;
      }

    char label[10];

    Point a;
    Point b;

    a.x = 15;
    a.y = 20;

    setTextScale(wdw,2,2);
    
    FAST const char *title = namer->paramToName(param);

    if (title == NULL)
      title = "Unknown";

    horText(wdw,a,title,WHITE1);
    
    a.x = 20;
    a.y = 55;                   // To center label.

    b.x = 120;
    b.y = 50;

    --tblsize;                  // Both must be decremented, since we
                                // start at zero.

    --offset;

    float step = (max - min) / (float)tblsize;
    float m = max;

    for (tblsize++; tblsize > 0; --tblsize, --offset)
      {
          frect(wdw,b,20,20,offset);

          switch(tblsize)       // This is predicated on 31 colors!!!!!
            {
              case 31:
                sprintf(label,"%d",iround(max));
                horText(wdw,a,label,WHITE);
                break;

              case 16:
                sprintf(label,"%d",iround(min));
                horText(wdw,a,label,WHITE);
                break;

              case 1:
                sprintf(label,"%d",iround((max + min) / 2.0));
                horText(wdw,a,label,WHITE);
                break;
              case 24:
              case 8:
                int i;
                if (m < 0.0)
                  i = iround(m - .5);
                else
                  i = iround(m);
                sprintf(label,"%d",i);
                horText(wdw,a,label,WHITE);
                break;

            }
          m -= step;
          a.y += 30;
          b.y += 30;
      }
}   

void Horiz::makeCellVector(CELLSPACING &spacing)
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

void Horiz::hashMarks(double meters)
{
    Point a;
    Point b;
    Point c;
    Point d;

    a.y = 0;
    b.y = Horiz::DATA_HEIGHT - 1;
    d.x = Horiz::DATA_WIDTH - 1;
    c.x = 0;

    double km = meters / 1000.0;           // Convert to km.
    float ppkm = (float)DATA_WIDTH / km; // Pixels/meter.

    FAST int l = (int)(ppkm + .5);

    if (km >= 500.0)
      l *= 100;                 // Draw every 100 km.
    else if (km >= 100.0)
      l *= 20;                  // Draw every 20 km.
    else if (km >= 20.0)
      l *= 10;                  // Draw every 10 km.
    else
      l *= 5;                   // Draw every 5 km.

    FAST int j = Horiz::DATA_WIDTH;

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

void Horiz::updateTitle()
{
    char tmp[10];

    sprintf(tmp,"%.2f",latitude[0]);

    char latlong[20];

    strcpy(latlong,tmp);
    strcat(latlong," Lat., ");

    sprintf(tmp,"%.2f",longitude[0]);
    strcat(latlong,tmp);
    strcat(latlong," Long.");

    Point a;
    a.x = 20;
    a.y = 45;

    horText(1,a,latlong,WHITE1);

    FAST int np = numOfParams;
    
    if (np > 1)
      {
          horText(3,a,latlong,WHITE1);

          if (np > 2)
            horText(5,a,latlong,WHITE1);
      }

    // Print altitude of interest.
    char altText[25] = "Altitude ";

    sprintf(&altText[9],"%.1f",altitude/1000.0);
    strcat(altText," km");

    a.y = 20;

    horText(1,a,altText,WHITE1);

    if (np > 1)
      {
          horText(3,a,altText,WHITE1);

          if (np > 2)
            horText(5,a,altText,WHITE1);
      }
}

void Horiz::undisplay(void)
{
    FAST Window *wdw = Wdw;

    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay(); wdw++;
    wdw->undisplay();
}

int Horiz::wdwOk(int wdw)
{
    FAST int np = numOfParams;

    if (wdw == 0 || wdw == 1 || wdw == 6)
      return(1);

    if (np > 1)
      {
          if (wdw == 2 || wdw == 3 || wdw == 7)
            return(1);

          if (np > 2)
            if (wdw == 4 || wdw == 5 || wdw == 8)
              return(1);
      }
    return(0);
}

Horiz::~Horiz(void)
{
}
