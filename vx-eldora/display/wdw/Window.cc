/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		Window.cc 
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.6  1992/03/17  20:17:02  thor
 * Fixed aPan to add in base offset of window memory.
 *
 * Revision 1.5  1992/03/16  21:30:06  thor
 * Added aPan method to allow raw panning.
 *
 * Revision 1.4  1991/12/02  18:00:06  thor
 * Fixed several bugs in pan method.
 *
 * Revision 1.3  1991/10/24  19:11:53  thor
 * Major rewrite of center method.
 *
 * Revision 1.2  91/01/02  08:50:55  thor
 * Made changes for VxWorks Version 5.0.
 * 
 * Revision 1.1  90/12/04  10:21:29  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        The Window class allows the creation and manipulation of
 * logical windows that are tied to underlying video memory. This
 * means that the attributes of the window, such as starting location,
 * width, etc., can be changed at any time. The changes are only
 * visible if the window is displayed.
 *
 */
#include "Window.hh"
#include "WindowP.hh"

extern "C" {
#include "stdioLib.h"
#include "taskLib.h"
};

#include "mitchFont.h"

static void printCR(WindowControlRegs &);

Window::Window(GraphicController *cntlr, int wdw, unsigned short x, 
	       unsigned short y, unsigned short xwidth, unsigned short yheight,
	       unsigned short xoff, unsigned short yoff)
{
    if (xwidth > MAX_X_WIDTH || yheight > MAX_Y_HEIGHT)
      {
	  fprintf(stderr,"Window width and/or height too great - %d x %d.\n",
		  xwidth,yheight);
	  exit(1);
      }

    bottomRight.x = xwidth - 1 + xoff; // Get the lower right corner.
    bottomRight.y = yheight - 1 + yoff;

    upperLeft.x = xoff;		// Get upper left corner.
    upperLeft.y = yoff;

    controlSetting.startx = x;
    controlSetting.starty = y;
    controlSetting.width = xwidth;
    controlSetting.height = yheight;
    controlSetting.xoffset = xoff;
    controlSetting.yoffset = yoff;

    controlSetting.viewPriority = 7;
    controlSetting.zoom = ZOOM1;

    displayState = NOT_DISPLAYED; // Window is not displayed by default.

    vmeLocation = cntlr->videoBufferAddr();

    gbd = cntlr;

    window = wdw;

    // Now the text defaults.
    textScaleX = 2;
    textScaleY = 1;

    backGroundColor = DONT_TOUCH;

    fontList = (char *)mitchFont;
}

void Window::init(GraphicController *cntlr, int wdw, unsigned short x, 
	       unsigned short y, unsigned short xwidth, unsigned short yheight,
	       unsigned short xoff, unsigned short yoff)
{
    if (xwidth > MAX_X_WIDTH || yheight > MAX_Y_HEIGHT)
      {
	  fprintf(stderr,"Window width and/or height too great - %d x %d.\n",
		  xwidth,yheight);
	  exit(1);
      }

    bottomRight.x = xwidth - 1 + xoff; // Get the lower right corner.
    bottomRight.y = yheight - 1 + yoff;

    upperLeft.x = xoff;		// Get upper left corner.
    upperLeft.y = yoff;

    controlSetting.startx = x;
    controlSetting.starty = y;
    controlSetting.width = xwidth;
    controlSetting.height = yheight;
    controlSetting.xoffset = xoff;
    controlSetting.yoffset = yoff;

    controlSetting.viewPriority = 7;
    controlSetting.zoom = ZOOM1;

    displayState = NOT_DISPLAYED; // Window is not displayed by default.

    vmeLocation = cntlr->videoBufferAddr();

    gbd = cntlr;

    window = wdw;

    // Now the text defaults.
    textScaleX = 2;
    textScaleY = 1;

    backGroundColor = DONT_TOUCH;

    fontList = (char *)mitchFont;
}

void Window::setPriority(FAST unsigned short priority)
{
    controlSetting.viewPriority |= priority <<3; // Only set global priority.

    if (displayState == DISPLAYED)
      gbd->priority(window,priority);
}

void Window::setZoom(FAST unsigned short zoom)
{
    FAST unsigned short newZ;

    switch(zoom)
      {
	case ZOOM1:
	case ZOOM2:
	case ZOOM4:
	case ZOOM8:
	  newZ = zoom;
	  break;

	default:
	  return;
      }

    if (displayState == DISPLAYED)
      {
	  newZ |= ENABLE;
	  gbd->zoom(window,newZ);
      }

    controlSetting.zoom = newZ;
}

void Window::display(void)
{
    if (displayState != DISPLAYED)
      {
	  displayState = DISPLAYED;
	  controlSetting.zoom |= ENABLE;
	  gbd->display(window,&controlSetting);
      }
}

void Window::undisplay(void)
{
    if (displayState != NOT_DISPLAYED)
      {
	  gbd->undisplay(window);
	  displayState = NOT_DISPLAYED;
      }
}

int Window::center(FAST Point center)
{
    FAST unsigned short zoom = getZoom();

    if (zoom == ZOOM1)		// Get the right zoom factor.
      return(OK);		// No panning allowed!
    
    FAST unsigned short width = (bottomRight.x - upperLeft.x) + 1;
    FAST unsigned short height = (bottomRight.y - upperLeft.y) + 1;

    FAST unsigned short x = center.x;
    FAST unsigned short y = center.y;

    if (zoom == ZOOM2)
      {
	  width /= 4;		// Calculate the half size of a window.
	  height /= 4;
      }
    else if (zoom == ZOOM4)
      {
	  width /= 8;
	  height /= 8;
	  x /= 2;		// Correct for previous zoom.
	  y /= 2;
      }
    else
      {
	  width /= 16;
	  height /= 16;
	  x /= 4;
	  y /= 4;
      }

    x += controlSetting.xoffset; // Put x,y at real location selected.
    y += controlSetting.yoffset;

    x -= width;			// Change from "center" to offsets of
				// upper left.
    y -= height;

    FAST unsigned short botX = bottomRight.x - (width * 2);

    botX++;			// Include all pixels.

    if (x > 0x7fff)		// Negative value?
      x = 0;
    else if (x > botX)		// Too close to edge.
      x = botX;

    FAST unsigned short botY = bottomRight.y - (height * 2);
    botY++;

    if (y > 0x7fff)
      y = 0;
    else if (y > botY)
      y = botY;

    controlSetting.xoffset = x;	// New x,y offsets.
    controlSetting.yoffset = y;

    if (displayState == DISPLAYED) // If on crt do update now!
      {
	  FAST long l = x << 16;

	  l |= y;

	  gbd->pan(window,l);
      }
    return(OK);
}    
    
int Window::pan(FAST Point panOrigin)
{
    FAST unsigned short x = panOrigin.x;
    FAST unsigned short y = panOrigin.y;
    FAST unsigned short width = (bottomRight.x - upperLeft.x) + 1;
    FAST unsigned short height = (bottomRight.y - upperLeft.y) + 1;

    FAST unsigned short zoom = getZoom();

    if (zoom == ZOOM1)
      {
	  width /= 2;
	  height /= 2;
      }
    if (zoom == ZOOM2)
      {
 	  x /= 2;
 	  y /= 2;
	  width /= 4;		// Calculate the half size of a window.
	  height /= 4;
      }
    else if (zoom == ZOOM4)
      {
	  x /= 4;
	  y /= 4;
	  width /= 8;		// Calculate the half size of a window.
	  height /= 8;
      }
    else if (zoom == ZOOM8)
      {
	  x /= 8;
	  y /= 8;
	  width /= 16;		// Calculate the half size of a window.
	  height /= 16;
      }

    x += controlSetting.xoffset;
    y += controlSetting.yoffset;

    x -= width;			// Change from "center" to offsets of
				// upper left.
    y -= height;

    FAST unsigned short botX = bottomRight.x - (width * 2);

    botX++;			// Include all pixels.

    if (x > 0x7fff)		// Negative value?
      x = 0;
    else if (x > botX)		// Too close to edge.
      x = botX;

    FAST unsigned short botY = bottomRight.y - (height * 2);
    botY++;

    if (y > 0x7fff)
      y = 0;
    else if (y > botY)
      y = botY;

    controlSetting.xoffset = x;	// New x,y offsets.
    controlSetting.yoffset = y;

    if (displayState == DISPLAYED)
      {
	  FAST long l = x << 16;

	  l |= y;
	  
	  gbd->pan(window,l);
      }

    return(OK);
}

void Window::aPan(FAST Point newOrigin)
{
    FAST unsigned short x = newOrigin.x;
    FAST unsigned short y = newOrigin.y;

    x += upperLeft.x;
    y += upperLeft.y;

    FAST long l = x << 16;

    l |= y;

    gbd->pan(window,l);

    controlSetting.xoffset = x;
    controlSetting.yoffset = y;
}

void Window::home(void)
{
    controlSetting.xoffset = upperLeft.x;
    controlSetting.yoffset = upperLeft.y;

    resize(bottomRight.x + 1 - upperLeft.x,bottomRight.y + 1 - upperLeft.y);

    if (displayState == DISPLAYED)
      {
	  FAST long l = upperLeft.x << 16;

	  l |= upperLeft.y;
	  
	  gbd->pan(window,l);
      }
}
    
int Window::move(FAST Point newOrigin)
{
    newOrigin.x += upperLeft.x;
    newOrigin.y += upperLeft.y;

    if (newOrigin.x >= MAX_X_WIDTH || newOrigin.y >= MAX_Y_HEIGHT)
      return(ERROR);
    
    controlSetting.startx = newOrigin.x;
    controlSetting.starty = newOrigin.y;
    
    if (displayState == DISPLAYED)
      {
	  FAST long l = newOrigin.x << 16;
	  
	  l |= newOrigin.y;
	  
	  gbd->screenMove(window,l);
      }
    return(OK);
}

int Window::resize(FAST unsigned short newWidth, FAST unsigned short newHeight)
{
    if (newWidth > MAX_X_WIDTH || newHeight > MAX_Y_HEIGHT)
      return(ERROR);

    controlSetting.width = newWidth;
    controlSetting.height = newHeight;

    if (displayState == DISPLAYED)
      {
	  FAST long newloc = newWidth << 16;
	  
	  newloc |= newHeight;
	  gbd->screenResize(window,newloc);
      }

    return(OK);
}

static void printCR(WindowControlRegs &regs)
{
    printf("startx = %d\n",regs.startx);
    printf("starty = %d\n",regs.starty);
    printf("width = %d\n",regs.width);
    printf("height = %d\n",regs.height);
    printf("xoff = %d\n",regs.xoffset);
    printf("yoff = %d\n",regs.yoffset);
    printf("priority = 0x%x\n",regs.viewPriority);
    printf("zoom = 0x%x\n\n",regs.zoom);
}
