/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Dual.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1993/09/03  17:10:25  thor
 * Fixed consts to avoid clashing with others.
 *
 * Revision 1.3  1993/09/03  17:10:25  thor
 * Fixed consts to avoid clashing with others.
 *
 * Revision 1.2  1993/08/31  16:39:41  thor
 * The first really working version.
 *
 * Revision 1.1  1992/11/06  20:20:20  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 *
 */
#include "Dual.hh"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

Dual::Dual(GraphicController *gbd,  unsigned short xoff, unsigned short yoff) :
	   TopData(gbd,0,0,0,DUAL_PLOT_WIDTH,DUAL_PLOT_HEIGHT,xoff,yoff),
	   BottomData(gbd,1,0,DUAL_PLOT_HEIGHT,DUAL_PLOT_WIDTH,
		      DUAL_PLOT_HEIGHT,xoff + DUAL_PLOT_WIDTH,yoff),
	   Tbl(gbd,2,DUAL_PLOT_WIDTH,0,DUAL_TBL_WIDTH,DUAL_TBL_HEIGHT,xoff,
	       yoff + DUAL_PLOT_MEM),
	   clk(gbd,3,0,0,xoff + (DUAL_TBL_WIDTH * 2),yoff + DUAL_PLOT_MEM)
{
    TopData.setPriority(0);
    BottomData.setPriority(0);
    Tbl.setPriority(0);

    clk.Display();

    // Set the base video memory addresses.
    videoMemory[0] = (unsigned char *)gbd->videoBufferAddr() + xoff +
      (yoff * 4096);
    videoMemory[1] = videoMemory[0] + DUAL_PLOT_WIDTH;

    curZoom = ZOOM1;		// Start on 1x.

    lastIndex = 1000;		// Force full beam on 1st drawing.

    FAST TrigData *ptr;

    if ((ptr = (TrigData *)malloc(sizeof(TrigData) * 720)) == 
	(TrigData *)NULL)
      {
	  fprintf(stderr,"Dual: Cannot allocate enough space.\n");
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

unsigned short Dual::GetZoom(void)
{
    return(TopData.getZoom());
}

void Dual::Display(void)
{
    TopData.display();
    Tbl.display();
    BottomData.display();
}

void Dual::Undisplay(void)
{
    TopData.undisplay();
    BottomData.undisplay();
    Tbl.undisplay();
}

void Dual::DrawTable(int set, float max, float min, FAST int param, 
			FAST int tblsize = 31)
{
    FAST Window *wdw = &Tbl;
    FAST int offset;
    FAST int top;
    FAST int base;

    if (set == A_SET)
      {
	  top = 0;
	  base = DUAL_TBL_HALF - 1;
	  offset = tblsize;
      }
    else if (set == B_SET)
      {
	  top = DUAL_TBL_HALF;
	  base = DUAL_TBL_HEIGHT;
	  offset = tblsize * 2;	// Bias into color lut.
      }
    else
      return;

    wdw->setTextScale(2,2);
    wdw->setTextBackGround(BLACK);
    
    FAST char *title = "Unknown";

    FAST int index = ParamToNum(param);

    if (index != -1)
      title = ParamTapeNames[index];

    Point a;
    Point b;

    a.x = 15;
    a.y = (DUAL_TBL_HALF / 2) + top;

    wdw->horText(a,title,WHITE);
    
    a.x = DUAL_TBL_WIDTH - 87;
    a.y = top + 15;

    b.x = DUAL_TBL_WIDTH - 31;;
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

void Dual::DrawTitle()
{
    FAST Window *wdw1 = &TopData;
    FAST Window *wdw2 = &BottomData;
    FAST Window *wdw3 = &Tbl;

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

//    strcat(label," km"); 
// What follows is a kludge to get around a gcc 2.2.2 bug.    
    int l = strlen(label);

    char *p = label + l;

    strcpy(p," km");

    wdw3->horText(a,label,WHITE);

    a.y += DUAL_TBL_HALF;

    wdw3->horText(a,label,WHITE);

    a.y = DUAL_TBL_HALF - 15;

    sprintf(label,"%4.1f",min);

//    strcat(label," km");
// What follows is a kludge to get around a gcc 2.2.2 bug.    
    l = strlen(label);

    p = label + l;

    strcpy(p," km");

    wdw3->horText(a,label,WHITE);

    a.y += DUAL_TBL_HALF;

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
    a.y = DUAL_TBL_HEIGHT - 40;

    wdw3->horText(a,div,WHITE);

    a.y -= DUAL_TBL_HALF;

    wdw3->horText(a,div,WHITE);

    FAST short limit = DUAL_PLOT_WIDTH - 1;

    a.x = ppkm;
    a.y = 0;
    b.x = ppkm;
    b.y = DUAL_PLOT_HEIGHT - 1;

    while (a.x < limit)
      {
	  wdw1->line(a,b,WHITE);
	  wdw2->line(a,b,WHITE);
	  a.x += ppkm;
	  b.x += ppkm;
      }

    limit = DUAL_PLOT_HEIGHT - 1;

    a.x = 0;
    a.y = ppkm;
    b.x = DUAL_PLOT_WIDTH - 1;
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
    b.x = DUAL_PLOT_WIDTH - 1;
    b.y = 0;

    wdw2->line(a,b,WHITE);

    a.y = DUAL_PLOT_HEIGHT - 1;
    b.y = DUAL_PLOT_HEIGHT - 1;

    wdw1->line(a,b,WHITE);

    a.y = DUAL_TBL_HALF - 1;
    b.x = DUAL_TBL_WIDTH - 1;
    b.y = DUAL_TBL_HALF - 1;

    wdw3->line(a,b,WHITE);

    a.y++;
    b.y++;

    wdw3->line(a,b,WHITE);
}

void Dual::Clear(void)
{
    TopData.clear();
    BottomData.clear();
    Tbl.clear();
}

void Dual::NextZoom(Point cursor)
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
#ifdef BROKEN_ZOOM
	  zoom = ZOOM1;
	  break;
#else
	  zoom = ZOOM8;
	  break;

	case ZOOM8:
	  zoom = ZOOM1;
	  break;
#endif	  
      }

    TopData.setZoom(zoom);
    BottomData.setZoom(zoom);

    if (zoom == ZOOM1)
      Home();
    else
      Center(cursor);

    curZoom = zoom;
}

void Dual::SetBounds(CELLSPACING &cs, float max, float min)
{
    maxAlt = max;
    minAlt = min;

    float ppm = (float)DUAL_PLOT_HEIGHT / (max - min);

    pixelsPerMeter = ppm;

    float f = ppm * cs.distToFirst;

    firstGate = fastround(f);

    radius = 2 * DUAL_PLOT_HEIGHT;

    conv->SetBeamSize(cs,radius,(1.0/ppm));
}

Dual::~Dual(void)
{
    free((char *)trigData);

    Clear();
    Undisplay();

    clk.clear();
    clk.undisplay();
}
