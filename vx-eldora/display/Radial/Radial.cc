/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Radial.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/04/08  20:28:42  thor
 * Initial revision
 *
 *
 * description:
 *        This file contains all but the drawing and inline methods
 * for the Radial class.
 *
 */
#include "Radial.hh"

extern "C" {
#include "stdioLib.h"
#include "memLib.h"
};

Radial::Radial(GraphicController *gbd, FAST int rad, FAST int nparams,
		 unsigned short xoff, unsigned short yoff)
{
    // Set the base video memory addresses.
    videoMemory[0] = gbd->videoBufferAddr() + xoff + (yoff * 4096);
    videoMemory[1] = videoMemory[0] + PLOT_WIDTH;
    videoMemory[2] = videoMemory[0] + (PLOT_HEIGHT * 4096);

    // Set up the offsets to each center point.
    videoBase[0] = videoMemory[0] + rad + (rad * 4096);
    videoBase[1] = videoMemory[1] + rad + (rad * 4096);
    videoBase[2] = videoMemory[2] + rad + (rad * 4096);

    numOfParams = nparams;
    curZoom = ZOOM1;		// Start on 1x.
    radius = rad;
    lastIndex = 1000;		// Force full beam on 1st drawing.

    // The color tables are located directly after the third parameter
    // data area.
    FAST unsigned short tbl_x = xoff + PLOT_WIDTH;
    FAST unsigned short tbl_y = yoff + PLOT_HEIGHT;

    Wdw[0].init(gbd,0,0,0,PLOT_WIDTH,PLOT_HEIGHT,xoff,yoff);
    Wdw[1].init(gbd,1,PLOT_WIDTH,0,TBL_WIDTH,TBL_HEIGHT,tbl_x,tbl_y);

    xoff += PLOT_WIDTH;
    tbl_x += TBL_WIDTH;

    Wdw[2].init(gbd,0,0,0,PLOT_WIDTH,PLOT_HEIGHT,xoff,yoff);
    Wdw[3].init(gbd,1,PLOT_WIDTH,0,TBL_WIDTH,TBL_HEIGHT,tbl_x,tbl_y);

    xoff -= PLOT_WIDTH;
    yoff += PLOT_HEIGHT;
    tbl_x += TBL_WIDTH;

    Wdw[4].init(gbd,0,0,0,PLOT_WIDTH,PLOT_HEIGHT,xoff,yoff);
    Wdw[5].init(gbd,1,PLOT_WIDTH,0,TBL_WIDTH,TBL_HEIGHT,tbl_x,tbl_y);

    FAST TrigData *ptr;

    if ((ptr = (TrigData *)malloc(sizeof(TrigData) * 720)) == 
	(TrigData *)NULL)
      {
	  fprintf(stderr,"Radial: Cannot allocate enough space.\n");
	  exit(1);
      }

    trigData = ptr;

    // We're using half a degree resolution. It must be understood
    // that 0 degrees in the radar system is straight up, 90 degrees
    // to the right. In display terms this corresponds to 270 & 0
    // degrees.
    double angle = 270.0 * (M_PI / 180.0);
    double inc = 0.5 * (M_PI / 180.0);
    
    for (FAST int i = 0; i < 720; i++, ptr++, angle += inc)
      {
	  ptr->cos = (int)(cos(angle) * 65536.0);
	  ptr->sin = (int)(sin(angle) * 65536.0);
      }
}

void Radial::expose(FAST int wdw)
{
    Wdw[wdw].expose();
}

void Radial::hide(FAST int wdw)
{
    Wdw[wdw].hide();
}

unsigned short Radial::getPriority(void)
{
    return(Wdw[0].getPriority());
}

int Radial::move(Point newOrigin)
{
    if (Wdw[0].move(newOrigin) != OK)
      return(ERROR);

    if (Wdw[2].move(newOrigin) != OK)
      return(ERROR);

    if (Wdw[4].move(newOrigin) != OK)
      return(ERROR);

    newOrigin.x += PLOT_WIDTH;

    if (Wdw[1].move(newOrigin) != OK)
      return(ERROR);

    if (Wdw[3].move(newOrigin) != OK)
      return(ERROR);

    if (Wdw[5].move(newOrigin) != OK)
      return(ERROR);
}

unsigned short Radial::getZoom(void)
{
    return(Wdw[0].getZoom());
}

void Radial::display(FAST int wdw)
{
    Wdw[wdw].display();
}

void Radial::undisplay(FAST int wdw)
{
    Wdw[wdw].undisplay();
}

void Radial::line(FAST int wdw, Point start, Point end, unsigned char color)
{
    Wdw[wdw].line(start,end,color);
}

void Radial::arc(FAST int wdw, Point center, Point left, Point right,
		  int rad, unsigned char color)
{
    Wdw[wdw].arc(center,left,right,rad,color);
}

void Radial::circle(FAST int wdw, Point center, int rad, unsigned char color)
{
    Wdw[wdw].circle(center,rad,color);
}

void Radial::fcircle(FAST int wdw, Point center, int rad, unsigned char color)
{
    Wdw[wdw].fcircle(center,rad,color);
}

void Radial::rect(FAST int wdw, Point corner, int width, int height, 
	      unsigned char color)
{
    Wdw[wdw].rect(corner,width,height,color);
}

void Radial::frect(FAST int wdw, Point left, Point right, unsigned char color)
{
    Wdw[wdw].frect(left,right,color);
}

void Radial::frect(FAST int wdw, Point corner, int width, int height, 
	       unsigned char color)
{
    Wdw[wdw].frect(corner,width,height,color);
}

void Radial::tri(FAST int wdw, Point one, Point two, Point three,
		  unsigned char color)
{
    Wdw[wdw].tri(one,two,three,color);
}

void Radial::ftri(FAST int wdw, Point one, Point two, Point three,
		   unsigned char color)
{
    Wdw[wdw].ftri(one,two,three,color);
}

void Radial::point(FAST int wdw, Point pt, unsigned char color)
{
    Wdw[wdw].point(pt,color);
}

void Radial::clear(FAST int wdw)
{
    Wdw[wdw].clear();
}

void Radial::setTextScale(FAST int wdw, int x, int y)
{
    Wdw[wdw].setTextScale(x,y);
}

void Radial::setFont(FAST char *font)
{
    Wdw[0].setFont(font);
    Wdw[1].setFont(font);
    Wdw[2].setFont(font);
    Wdw[3].setFont(font);
    Wdw[4].setFont(font);
    Wdw[5].setFont(font);
}

void Radial::setTextBackGround(FAST int wdw, int color)
{
    Wdw[wdw].setTextBackGround(color);
}

void Radial::horText(FAST int wdw, Point start, char *text,
		      unsigned char color)
{
    Wdw[wdw].horText(start,text,color);
}

void Radial::horCenteredText(FAST int wdw, Point start, char *text, 
			 unsigned char color)
{
    Wdw[wdw].horCenteredText(start,text,color);
}

void Radial::vertText(FAST int wdw, Point start, char *text,
		       unsigned char color)
{
    Wdw[wdw].vertText(start,text,color);
}

void Radial::vertCenteredText(FAST int wdw, Point start, char *text, 
			 unsigned char color)
{
    Wdw[wdw].vertCenteredText(start,text,color);
}

void Radial::drawTable(int set, float max, float min, FAST int param, 
			FAST int tblsize = 31)
{
    FAST int wdw;
    FAST int offset;

    if (set == A_SET)
      {
	  wdw = 1;
	  offset = tblsize;
      }
    else if (set == B_SET)
      {
	  wdw = 3;
	  offset = tblsize * 2;	// Bias into color lut.
      }
    else
      {
	  wdw = 5;
	  offset = tblsize * 3;
      }

    char label[10];

    Point a;
    Point b;

    a.x = 15;
    a.y = 20;

    setTextScale(wdw,2,2);
    
    FAST char *title = "Unknown Parameter";
    FAST char *units = "Scale is Unknown";

    switch(param)
      {
	case REFLECTIVITY:
	  title = "Reflectivity";
	  units = "dbZ";
	  break;

	case VELOCITY:
	  title = "Velocity";
	  units = "meters per second";
	  break;

	case SPEC_WIDTH:
	  title = "Spectral Width";
	  units = NULL;
	  break;

	case LINEAR_POWER:
	  title = "Linear Power";
	  units = 0;
	  break;

	case REAL_R1:
	  title = "Real R(1)";
	  units = 0;
	  break;

	case IMAG_R1:
	  title = "Imaginary R(1)";
	  units = 0;
	  break;

	case REAL_R2:
	  title = "Real R(2)";
	  units = 0;
	  break;

	case IMAG_R2:
	  title = "Imaginary R(2)";
	  units = 0;
	  break;
      }

    horText(wdw,a,title,WHITE);
    
    a.x = 20;
    a.y = 50;

    b.x = 120;
    b.y = 50;

    --tblsize;			// Both must decremented, since we
				// start at zero.
    --offset;

    float step = (max - min) / (float)tblsize;

    for (; tblsize > 0; --tblsize, --offset)
      {
	  frect(wdw,b,20,20,offset);

	  sprintf(label,"%6.1f",max);

	  horText(wdw,a,label,WHITE);

          max -= step;
          a.y += 30;
          b.y += 30;
      }
    sprintf(label,"%6.1f",min);

    horText(wdw,a,label,WHITE);
    frect(wdw,b,20,20,offset);

    if (units != NULL)
      {
	  a.x = 175;
	  a.y = 512;
	  vertCenteredText(wdw,a,units,WHITE);
      }
}

void Radial::drawTitle(FAST int set, FAST int param, float distance)
{
    FAST int wdw;

    if (set == A_SET)
      wdw = 0;
    else if (set == B_SET)
      wdw = 2;
    else
      wdw = 4;

    Point a;

    a.x = 15;
    a.y = 20;

    setTextScale(wdw,2,2);
    
    FAST char *title = "Unknown Parameter";

    switch(param)
      {
	case REFLECTIVITY:
	  title = "Reflectivity";
	  break;

	case VELOCITY:
	  title = "Velocity";
	  break;

	case SPEC_WIDTH:
	  title = "Spectral Width";
	  break;

	case LINEAR_POWER:
	  title = "Linear Power";
	  break;

	case REAL_R1:
	  title = "Real R(1)";
	  break;

	case IMAG_R1:
	  title = "Imaginary R(1)";
	  break;

	case REAL_R2:
	  title = "Real R(2)";
	  break;

	case IMAG_R2:
	  title = "Imaginary R(2)";
	  break;
      }

    horText(wdw,a,title,WHITE);

    FAST int rad = radius;

    a.x = rad;
    a.y = rad;

    // Draw hash circles.
    circle(wdw,a,rad/4,WHITE);
    taskDelay(1);
    circle(wdw,a,rad/2,WHITE);
    taskDelay(1);
    circle(wdw,a,(3 * rad)/4,WHITE);
    taskDelay(1);
    circle(wdw,a,rad,WHITE);

    Point b;

    // Draw cross hairs.
    a.x = 0;
    b.x = 2 * rad;
    b.y = rad;

    line(wdw,a,b,WHITE);

    a.x = rad;
    a.y = 0;
    b.x = rad;
    b.y = 2 * rad;

    line(wdw,a,b,WHITE);

    float hashDist = distance / 4.0;

    hashDist *= cos(BEAM_OFFSET); // Correct for offset from perpendicular.

    a.x = 0;
    a.y = 975;

    char hashText[20];

    sprintf(hashText,"%9.4f km",hashDist);

    horText(wdw,a,hashText,WHITE);

    a.x = 770;
    
    horText(wdw,a,"per division",WHITE);
}
    
void Radial::nextZoom(Point cursor)
{
    FAST int zoom = curZoom;

    switch(zoom)
      {
	case ZOOM1:
	  cursor.x *= 2;
	  cursor.y *= 2;
	  zoom = ZOOM2;
	  break;

	case ZOOM2:
	  cursor.x *= 2;
	  cursor.y *= 2;
	  zoom = ZOOM4;
	  break;

	case ZOOM4:
	  zoom = ZOOM1;
	  break;
      }

    Wdw[0].setZoom(zoom);
    Wdw[2].setZoom(zoom);
    Wdw[4].setZoom(zoom);

    if (zoom == ZOOM1)
      homeAll();
    else
      centerAll(cursor);

    curZoom = zoom;
}

Radial::~Radial(void)
{
    free((char *)trigData);

    FAST int j = 6;

    for (FAST i = 0; i < j; i++)
      {
	  clear(i);
	  undisplay(i);
      }
}