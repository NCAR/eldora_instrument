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

#include "stdioLib.hh"
#include "taskLib.hh"

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
    else if (zoom == ZOOM2)
      {
	  zoom = 2;
	  center.x >>= 1;
	  center.y >>= 1;
      }
    else if (zoom == ZOOM4)
      {
	  zoom = 3;
	  center.x >>= 2;
	  center.y >>= 2;
      }	  
    else			// == ZOOM8.
      {
	  zoom = 4;
	  center.x >>= 3;
	  center.y >>= 3;
      }

    center.x += controlSetting.xoffset;
    center.y += controlSetting.yoffset;

    FAST unsigned short width = (bottomRight.x - upperLeft.x) + 1;
    FAST unsigned short height = (bottomRight.y - upperLeft.y) + 1;

    width /= 2;
    height /= 2;

    FAST unsigned short hw = width >> zoom;
    FAST unsigned short hh = height >> zoom;
    

    FAST int x = center.x & 0xffff;
    FAST int y = center.y & 0xffff;

    x -= hw;
    y -= hh;

    zoom -= 2;

    hw = width >> zoom;
    hh = height >> zoom;


    if (x < upperLeft.x)
      x = upperLeft.x;

    if (x > bottomRight.x + 1 - hw)
      x = bottomRight.x + 1 - hw;

    if (y < upperLeft.y)
      y = upperLeft.y;

    if (y > bottomRight.y + 1 - hh)
      y = bottomRight.y + 1 - hh;

    controlSetting.xoffset = x;
    controlSetting.yoffset = y;

    resize(width * 2,height * 2);

    if (displayState == DISPLAYED)
      {
	  FAST long l = x << 16;

	  l |= y;
	  
	  gbd->pan(window,l);
      }

    return(OK);
}    
    
int Window::pan(FAST Point panOrigin)
{
    panOrigin.x += controlSetting.xoffset;
    panOrigin.y += controlSetting.yoffset;

    if (panOrigin.x >= bottomRight.x  || panOrigin.y >= bottomRight.y)
      return(ERROR);

    controlSetting.xoffset = panOrigin.x;
    controlSetting.yoffset = panOrigin.y;

    if (displayState == DISPLAYED)
      {
	  FAST long l = panOrigin.x << 16;

	  l |= panOrigin.y;
	  
	  gbd->pan(window,l);
      }

    return(OK);
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
