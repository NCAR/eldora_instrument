/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		GraphicController.cc 
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:51:14  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  91/01/02  08:50:12  thor
 * Made changes for VxWorks Version 5.0.
 * 
 * Revision 1.1  90/12/04  10:20:47  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        These are the various window operations that the TS2000
 * chips can perform. One can pan, zoom, display, undisplay, move,
 * resize and change the viewing priority of any window. 
 *
 */
#define GRAPHIC_CTLR_PRIVATE
#include "GraphicController.hh"

int GraphicController::setGraphicOp(FAST int op)
{
    FAST int last = lastQpdmOp;

    lastQpdmOp = (unsigned short)op;

    return(last);
}

void GraphicController::priority(FAST int window, FAST unsigned short 
				    priority)
{
    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);
   *windows[window].priority = priority;
    semGive(sem);
}

void GraphicController::display(FAST int window, FAST WindowControlRegs 
				*settings)
{
    FAST long *data = (long *)settings;

    FAST long *ptr = windows[window].base;

    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);

    *ptr++ = *data++;		// Since we're overwriting current
				// logical window, all registers get set.
    *ptr++ = *data++;
    *ptr++ = *data++;
    *ptr = *data;

    semGive(sem);
}

void GraphicController::undisplay(FAST int window)
{
    FAST SEM_ID sem = accessSem;
    FAST unsigned short *ptr = windows[window].zoom;

    semTake(sem,WAIT_FOREVER);
    
    *ptr = 0;

    semGive(sem);
}

void GraphicController::screenMove(FAST int window, FAST long newLocation)
{
    FAST SEM_ID sem = accessSem;

    FAST long *ptr = windows[window].base;

    semTake(sem,WAIT_FOREVER);

    *ptr = newLocation;

    semGive(sem);
}

void GraphicController::screenResize(FAST int window, FAST long newLocation)
{
    FAST SEM_ID sem = accessSem;

    FAST long *ptr = windows[window].scrSize;

    semTake(sem,WAIT_FOREVER);

    *ptr = newLocation;

    semGive(sem);
}

void GraphicController::pan(FAST int window, FAST long newLocation)
{
    FAST SEM_ID sem = accessSem;

    FAST long *ptr = windows[window].offsets;

    semTake(sem,WAIT_FOREVER);

    *ptr = newLocation;

    semGive(sem);
}

void GraphicController::zoom(FAST int window, FAST unsigned short zoom)
{
    FAST SEM_ID sem = accessSem;
    FAST unsigned short *ptr = windows[window].zoom;

    semTake(sem,WAIT_FOREVER);

    *ptr = zoom;

    semGive(sem);
}

int GraphicController::setMask(FAST int mask)
{
    FAST unsigned char *reg = maskReg;
    
    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);

    FAST int oldMask = *reg & 0xff; // Get current bitmask.

   *reg = mask;

    long data;
    FAST unsigned short *ptr = (unsigned short *)&data;

    mask = ~mask;

    *ptr++ = SET_ACT_BITS;
    *ptr = mask;

    semGive(sem);

    qpdmCmd(&data,1);

    return(oldMask);
}
