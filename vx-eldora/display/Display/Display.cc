//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Display.cc
//	Original Author: Richard E. K. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        This file contains all but the drawing and inline methods
// for the Display class.
//
//
#include "vxWorks.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "taskLib.h"

#pragma implementation

#include "Display.hh"

Display::Display(GraphicController *gbd)
{
    agc = gbd;
    curZoom = ZOOM1;		// Start on 1x.
    lastIndex = 1000;		// Force full beam on 1st drawing.

    FAST TrigData *ptr;

    if ((ptr = new TrigData[720]) == (TrigData *)NULL)
      {
	  cout << "Display: Cannot allocate enough space." << endl;
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

void Display::setPriority(FAST unsigned short priority)
{
    FAST Window *w = Wdw;

    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
    w->setPriority(priority); w++;
}

unsigned short Display::getPriority(void)
{
    return(Wdw[0].getPriority());
}

unsigned short Display::getZoom(void)
{
    return(Wdw[0].getZoom());
}

void Display::line(FAST int wdw, Point start, Point end, unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].line(start,end,color);
}

void Display::arc(FAST int wdw, Point center, Point left, Point right,
		  int rad, unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].arc(center,left,right,rad,color);
}

void Display::circle(FAST int wdw, Point center, int rad, unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].circle(center,rad,color);
}

void Display::fcircle(FAST int wdw, Point center, int rad, unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].fcircle(center,rad,color);
}

void Display::rect(FAST int wdw, Point corner, int width, int height, 
	      unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].rect(corner,width,height,color);
}

void Display::frect(FAST int wdw, Point left, Point right, unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].frect(left,right,color);
}

void Display::frect(FAST int wdw, Point corner, int width, int height, 
	       unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].frect(corner,width,height,color);
}

void Display::tri(FAST int wdw, Point one, Point two, Point three,
		  unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].tri(one,two,three,color);
}

void Display::ftri(FAST int wdw, Point one, Point two, Point three,
		   unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].ftri(one,two,three,color);
}

void Display::point(FAST int wdw, Point pt, unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].point(pt,color);
}

void Display::clear(FAST int wdw)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].clear();
}

void Display::setTextScale(FAST int wdw, int x, int y)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].setTextScale(x,y);
}

void Display::setFont(FAST const char *font)
{
    FAST Window *ptr = Wdw;
    
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);
    ptr++;
    ptr->setFont(font);

}

void Display::setTextBackGround(FAST int wdw, int color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].setTextBackGround(color);
}

void Display::horText(FAST int wdw, Point start, const char *text,
		      unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].horText(start,text,color);
}

void Display::horCenteredText(FAST int wdw, Point start, const char *text, 
			 unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].horCenteredText(start,text,color);
}

void Display::vertText(FAST int wdw, Point start, const char *text,
		       unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].vertText(start,text,color);
}

void Display::vertCenteredText(FAST int wdw, Point start, const char *text, 
			 unsigned char color)
{
    if (!wdwOk(wdw)) return;
    Wdw[wdw].vertCenteredText(start,text,color);
}

void Display::drawTable(int set, float max, float min, FAST int param, 
			FAST int tblsize = 31)
{
    FAST int wdw;
    FAST int offset;

    if (set == Display::A_SET)
      {
	  wdw = 1;
	  offset = tblsize;
      }
    else if (set == Display::B_SET)
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
    
    FAST const char *title = namer->paramToName(param);

    if (title == NULL)
      title = "Unknown";

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
		sprintf(label,"%d",iround(max));
		horText(wdw,a,label,WHITE);
		break;
	      case 16:
		sprintf(label,"%d",iround((max + min) / 2.0));
		horText(wdw,a,label,WHITE);
		break;
	      case 1:
		sprintf(label,"%d",iround(min));
		horText(wdw,a,label,WHITE);
		break;
	      case 24:
	      case 8:
		int i;
		if (i < 0.0)
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

void Display::nextZoom(Point cursor)
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
	  zoom = ZOOM8;
	  break;

	case ZOOM8:
	  zoom = ZOOM1;
	  break;
      }

    Wdw[0].setZoom(zoom);
    Wdw[2].setZoom(zoom);
    Wdw[4].setZoom(zoom);

    if (zoom == ZOOM1)
      home();
    else
      center(cursor);

    curZoom = zoom;
}

void Display::switchSets(void)
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
		
		Wdw[2].display();
		Wdw[3].display();
	    }
      }
    else if (dset == Display::B_SET)
      {
	  Wdw[2].undisplay();
	  Wdw[3].undisplay();

	  if (nparams == 2)
	    {
		dset = Display::A_SET;

		Wdw[0].display();
		Wdw[1].display();
	    }
	  else
	    {
		dset = Display::C_SET;

		Wdw[4].display();
		Wdw[5].display();
	    }
      }
    else
      {
	  dset = Display::A_SET;
	  Wdw[4].undisplay();
	  Wdw[5].undisplay();

	  Wdw[0].display();
	  Wdw[1].display();
      }
    displayedSet = dset;
}


int Display::wdwOk(FAST int wdw)
{
    FAST int i = numOfWdws;

    if ((wdw >= i) || (wdw < 0))
      return(0);

    return(1);
}

void Display::undisplay(void)
{
    FAST Window *ptr = Wdw;
    
    FAST int nw = numOfWdws;

    while (nw--)
	{
	    ptr->undisplay();
	    ptr++;
	}
}

Display::~Display(void)
{
    delete [] trigData;

    FAST int nw = numOfWdws;

    FAST Window *ptr = Wdw;

    while (nw--)
	{
	    ptr->clear();
	    ptr->undisplay();
	    ptr++;
	}
}
