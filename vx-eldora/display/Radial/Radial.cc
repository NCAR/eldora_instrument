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
 * Revision 1.15  1992/05/04  16:46:58  thor
 * Added/changed code to handle display of parameter values hopefully to
 * most people's liking.
 *
 * Revision 1.14  1992/02/27  19:25:12  thor
 * Changed manner of getting correct title to use table.
 *
 * Revision 1.13  1992/02/05  20:44:06  thor
 * Faster firstGate calc.
 *
 * Revision 1.12  1992/02/05  19:39:32  thor
 * Changed code to put any radius circle at center of data window.
 *
 * Revision 1.11  1991/12/03  18:03:14  thor
 * Changed hash circles to draw at 5 or 10 km depending on max distance.
 *
 * Revision 1.10  1991/12/03  17:33:30  thor
 * Changed table drawing to use integer steps & less text.
 *
 * Revision 1.9  1991/11/14  18:20:22  thor
 * Fixed incorrect calculation of first gate index.
 *
 * Revision 1.8  1991/10/24  15:46:42  thor
 * Changed nextZoom to eliminate extraneous cursor location diddling.
 *
 * Revision 1.7  1991/10/22  18:10:08  thor
 * Fixed signal/noise ratio text to fit screen. Added NCP (normalized
 * coherent poer).
 *
 * Revision 1.6  1991/10/21  18:08:37  thor
 * Fixed to draw text on black background, divide max distance by 1000.
 *
 * Revision 1.5  1991/10/09  18:03:03  thor
 * Fixed name for reflectivity code.
 *
 * Revision 1.4  1991/06/13  17:52:26  thor
 * Added SetBounds method.
 * Rewrote drawTitle method to use radar type.
 * Added signal to noise ratio text.
 *
 * Revision 1.3  1991/05/06  15:08:51  thor
 * Added embedded Clock class for time display.
 *
 * Revision 1.2  1991/04/29  18:47:43  thor
 * Fixed angles so we match radar conventions.
 *
 * Revision 1.1  1991/04/08  20:28:42  thor
 * Initial revision
 *
 *
 * description:
 *        This file contains all but the drawing and inline methods
 * for the Radial class.
 *
 */
extern "C" {
#include "vxWorks.h"
#include "stdioLib.h"
#include "string.h"
#include "memLib.h"
};

#include "Radial.hh"

Radial::Radial(GraphicController *gbd, FAST int rad, FAST int nparams,
	       unsigned short xoff, unsigned short yoff) :
	       clk(gbd,3,PLOT_WIDTH,1024 - (CLKHEIGHT + 8),
		   xoff + PLOT_WIDTH + (3 * TBL_WIDTH),
		   yoff + PLOT_HEIGHT)
{
    clk.Display();

    // Set the base video memory addresses.
    videoMemory[0] = gbd->videoBufferAddr() + xoff + (yoff * 4096);
    videoMemory[1] = videoMemory[0] + PLOT_WIDTH;
    videoMemory[2] = videoMemory[0] + (PLOT_HEIGHT * 4096);

    // Set up the offsets to each center point.
    FAST int center = (PLOT_WIDTH / 2) - 1;
    videoBase[0] = videoMemory[0] + center + (center * 4096);
    videoBase[1] = videoMemory[1] + center + (center * 4096);
    videoBase[2] = videoMemory[2] + center + (center * 4096);

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

    return(OK);
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
    setTextBackGround(wdw,BLACK);
    
    FAST char *title = "Unknown";

    FAST int index = ParamToNum(param);

    if (index != -1)
      title = ParamTapeNames[index];

    horText(wdw,a,title,WHITE);
    
    a.x = 40;
    a.y = 55;			// The extra 5 pixels centers the label.

    b.x = 130;
    b.y = 50;

    --tblsize;			// Both must decremented, since we
				// start at zero.
    --offset;

    float step = (max - min) / (float)tblsize;
    float m = max;

    for (tblsize++; tblsize > 0; --tblsize, --offset)
      {
	  frect(wdw,b,20,20,offset);

	  switch(tblsize)	// This is predicated on 31 colors!!!!!
	    {
	      case 31:
	      case 25:
	      case 20:
	      case 16:
	      case 10:
	      case 5:
	      case 1:
		int i = iround(m + .5);
		sprintf(label,"%d",i);
		horText(wdw,a,label,WHITE);
		break;
	    }
          m -= step;
          a.y += 30;
          b.y += 30;
      }
}

void Radial::drawTitle(FAST int set, FAST int radar)
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
    
    FAST char *title = "Unknown Display";

    if (radar == FORWARD_RADIAL)
      title = "Forward Radar";
    else if (radar == AFT_RADIAL)
      title = "Aft Radar";

    horText(wdw,a,title,WHITE);

    FAST int rad = radius;

    FAST int center = (PLOT_WIDTH / 2) - 1;

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

    char tmpText[20];

    sprintf(tmpText,"%9.2f",max);

    strcat(tmpText," km");
    
    horText(wdw,a,tmpText,WHITE);
}
    
void Radial::nextZoom(Point cursor)
{
    FAST int zoom = curZoom;

    switch(zoom)
      {
	case ZOOM1:
	  zoom = ZOOM2;
	  break;

	case ZOOM2:
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
    clk.clear();
    clk.undisplay();
}

void Radial::SetBounds(float max, float first)
{
    maxDistance = max;

    float rad = radius;

    float step = max / rad;

    FAST int fg = (int)(first / step);

    firstGate = fg;
}
