/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:51:14  thor
 * First offical ELDORA release!
 *
 * Revision 1.3  1991/12/02  17:59:14  thor
 * Changed ISR to wait .25 second between events.
 *
 * Revision 1.2  1991/01/02  08:50:48  thor
 * Made changes for VxWorks Version 5.0.
 *
 * Revision 1.1  90/12/04  10:21:18  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This is the methods file for the AGC Mouse. As always
 * initialization is a pain. The default state after initialization is:
 *
 *        Cursor enabled and set to location 0,0.
 *        Interrupt for button down enabled.
 *        Gearing is 8, high gear is 40.
 *
 * To avoid attempts to modify the mouse state simultaneously, the
 * necessary code is guarded by a semaphore. The ISR routine is not
 * visible to the public, since it only signals if an interrupt has
 * occurred. The waitOnInterrupt routine blocks on this semaphore and
 * returns the cause of the interrupt. 
 *
 */
#define MOUSE_CLASS
#include "Mouse.hh"

extern "C" {
#include "intLib.h"
#include "stdioLib.h"
#include "string.h"
#include "tickLib.h"
};

static void mouseISR(MouseISRData *);

Mouse::Mouse(FAST void *addr, Point firstXY, FAST long *cursorBits, int vector)
{
    mouseBase = addr + MOUSE_OFFSET;
    cursorBase = addr + CURSOR_OFFSET;
    crosshairBase = addr + CROSSHAIR_OFFSET;
    kbdBase = addr + KBD_OFFSET;

    cursorImage = (long *)(addr + CURSOR_IMAGE_OFFSET);

    interruptMCU = (unsigned char *)(addr + MCU_INT_OFFSET);
    mcuCmd = (unsigned char *)(addr + MCU_CMD_OFFSET);
    buttonState = (unsigned char *)(addr + BUTTON_OFFSET);
    kbdHead = (unsigned char *)(addr + KBD_HEAD_OFFSET);
    kbdTail = kbdHead + 1;

    kbdQueue = (keyEvent *)(addr + KBD_QUEUE_OFFSET);

    curX = (unsigned short *)(addr + CURX_OFFSET);

    isrData.clearMe = (unsigned char *)(addr + WHY_OFFSET);

// Download the cursor pattern.

    bcopy((char *)cursorBits,(char *)cursorImage,CURSOR_SIZE);

// Now create the needed semaphores.
    accessSem = semBCreate(SEM_Q_FIFO,SEM_FULL);

    isrData.sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

// Now link ISR to vector.
    vector &= 0xf0;		// First strip off AGC controlled bits.
    vector += 8;		// Now add offset for MCU interrupt

    if (intConnect((VOIDFUNCPTR *)(vector*4),(VOIDFUNCPTR)mouseISR,
		   (int)&isrData) == ERROR)
      {
	  fprintf(stderr,
		  "Failed to connect to interrupt vector %d. Exiting.\n");
	  exit(1);
      }

// Now initialize the MCU.
    FAST unsigned char *cinit = (unsigned char *)(mouseBase + MOUSE_INT_OFF);

    *cinit++ = BUTTON_DOWN;	// Set up mouse interrupt conditions.

    FAST unsigned short *sinit = (unsigned short *)(cinit);

    *sinit++ = 0;		// Set min/max positions.
    *sinit++ = 0;
    *sinit++ = 0x4ff;		// Stop moving at 1279,1023.
    *sinit++ = 0x3ff;

    cinit = (unsigned char *)sinit;

    *cinit++ = XGEAR;		// Set gearing (i.e. mouse speed).
    *cinit++ = YGEAR;
    *cinit++ = X_HI_GEAR;
    *cinit = Y_HI_GEAR;

    cinit = (unsigned char *)cursorBase;
    
    *cinit++ = CURSOR_ENABLE;	// Turn on cursor.

    sinit = (unsigned short *)(++cinit);

    *sinit++ = X_SLEW;		// Needed by Bt431 cursor chip.
    *sinit++ = (unsigned short)Y_SLEW;

    *sinit++ = firstXY.x;	// This is where the cursor will appear.
    *sinit = firstXY.y;

    cinit = (unsigned char *)crosshairBase;

    *cinit++ = 0;		// No crosshair yet.

    sinit = (unsigned short *)(++cinit);

    *sinit++ = X_SLEW;
    *sinit = (unsigned short)Y_SLEW;

    cinit = (unsigned char *)(crosshairBase + CROSS_WIDTH_OFF);

    *cinit = CROSS_WIDTH;

//  No keyboard for now!
//     cinit  = (unsigned char *)(kbdBase + KBD_CMD_OFF);
// 
//     *cinit++ = KBD_RESET;	// Get keyboard ready.
// 
//     *cinit = KBD_INT_MASK;	// Interrupt on key press.

    volatile FAST unsigned char *mcuInt = interruptMCU;
    FAST unsigned char *cmdReg = mcuCmd;

    // To send the AGC MCU a command, first write command type to
    // mcuCmd and then interrupt the MCU by writing any value to
    // interruptMCU.

    *cmdReg = CURSOR_PARAM_CHANGE;
    *mcuInt = 1;
    taskDelay(10);

    *cmdReg = CURSOR_XY_CHANGE;
    *mcuInt = 2;
    taskDelay(10);

    *cmdReg = CROSSHAIR_PARAM_CHANGE;
    *mcuInt = 3;
    taskDelay(10);


    *isrData.clearMe = 0;	// Must be set to 0 for MCU to interrupt host.
}

void Mouse::enableCursor(void)
{
    FAST unsigned char *en = (unsigned char *)cursorBase;
    FAST unsigned char *dis = (unsigned char *)crosshairBase;
    FAST unsigned char *mcuInt = interruptMCU;
    FAST unsigned char *cmd = mcuCmd;

    *en = CURSOR_ENABLE;
    *dis = ~CROSSHAIR_ENABLE;

    setCursorXY(getXY());	// Make sure we are at correct location.

    *cmd = CURSOR_PARAM_CHANGE;
    *mcuInt = 1;
    *cmd = CROSSHAIR_PARAM_CHANGE;
    *mcuInt = 2;
}

void Mouse::setCursorXY(Point newLocation)
{
    FAST unsigned short *ptr = (unsigned short *)(cursorBase + CURSOR_SET_OFF);
    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);

    *ptr++ = newLocation.x;

    *ptr = newLocation.y;

    FAST unsigned char *mcuInt = interruptMCU;
    FAST unsigned char *cmd = mcuCmd;

    *cmd = CURSOR_XY_CHANGE;
    *mcuInt = 1;

    semGive(sem);
}

void Mouse::setCursorImage(FAST long *cursorBits)
{
    FAST long *target = cursorImage;
    FAST unsigned char *mcuInt = interruptMCU;
    FAST unsigned char *cmd = mcuCmd;
    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);

    bcopy((char *)cursorBits,(char *)target,CURSOR_SIZE);

    *cmd = CURSOR_PARAM_CHANGE;
    *mcuInt = 1;

    semGive(sem);
}

void Mouse::enableCrosshair(void)
{
    FAST unsigned char *en = (unsigned char *)crosshairBase;
    FAST unsigned char *dis = (unsigned char *)cursorBase;
    FAST unsigned char *mcuInt = interruptMCU;
    FAST unsigned char *cmd = mcuCmd;

    *en = CROSSHAIR_ENABLE;
    *dis = ~CURSOR_ENABLE;

    setCrosshairXY(getXY());

    *cmd = CROSSHAIR_PARAM_CHANGE;
    *mcuInt = 1;    
    *cmd = CURSOR_PARAM_CHANGE;
    *mcuInt = 2;    
}

void Mouse::setCrosshairXY(Point newLocation)
{
    FAST unsigned short *ptr = (unsigned short *)(crosshairBase +
						  CROSSHAIR_SET_OFF);
    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);

    *ptr++ = newLocation.x;

    *ptr = newLocation.y;

    FAST unsigned char *mcuInt = interruptMCU;
    FAST unsigned char *cmd = mcuCmd;

    *cmd = CURSOR_XY_CHANGE;
    *mcuInt = 1;

    semGive(sem);
}

// getNextKey() will return the next key event in the buffer -
// according to what the indexes say.
int Mouse::getNextKey(void)
{
    if (keyboardEmpty())
      return(NO_KEY_EVENT);

    FAST SEM_ID sem = accessSem;
    FAST keyEvent *kev = kbdQueue;
    FAST unsigned char *index = kbdTail;

    semTake(sem,WAIT_FOREVER);

    FAST unsigned char loc = *index;

    kev += loc;			// Get offset to end of queue.

    if (!--loc)			// Decrement tail index. If == 0, then
      loc = KEY_BUF_SIZE - 1;	// roll over.

    *index = loc;

    semGive(sem);

    return(kev->keyCode & 0xff);
}

Point Mouse::getXY(void)
{
    FAST unsigned short *ptr = curX;
    Point curPt;

    curPt.x = *ptr++;
    curPt.y = *ptr;

    return(curPt);
}

int Mouse::waitOnInterrupt(void)
{
    semTake(isrData.sem,WAIT_FOREVER);

    return(isrData.interruptReason);
}

void Mouse::disableInterrupts(FAST int mask)
{
    enableInterrupts(~mask);
}
    
void Mouse::enableInterrupts(FAST int mask)
{
    FAST unsigned char *mouse = (unsigned char *)(mouseBase + MOUSE_INT_OFF);
    FAST unsigned char *kbd = (unsigned char *)(mouseBase + MOUSE_INT_OFF);
    FAST SEM_ID sem = accessSem;

    semTake(sem,WAIT_FOREVER);
    
    *mouse = mask & (BUTTON_UP | BUTTON_DOWN);	// Only button events allowed.
    *kbd = mask & 1;				// Only one keyboard case.

    semGive(sem);
}

static void mouseISR(FAST MouseISRData *isr)
{
    FAST unsigned char *ptr = isr->clearMe;

#ifdef NO_DEBOUNCE
    isr->interruptReason = *ptr & 0xff;	// Get interrupt cause.

    *ptr = 0;			// Clear this to allow more MCU interrupts.

    semGive(isr->sem);		// Signal user level code.
#else

    static int lastTick = 0;	// Previous time.

    FAST int ctick = tickGet();	// Current time.

    if (ctick - lastTick < 25)	// If not .25 sec, don't tell user.
      {
	  *ptr = 0;
	  return;
      }
    else
      {
	  lastTick = ctick;
	  
	  isr->interruptReason = *ptr & 0xff;
	  
	  *ptr = 0;

	  semGive(isr->sem);
      }
#endif
}
