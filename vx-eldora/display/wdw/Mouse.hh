/*
 *	$Id$
 *
 *	Module:		 Mouse.hh
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
 *        The class Mouse implements methods that control a 3 button
 * mouse and a keyboard. The AGC has two pointer images that can be
 * used - the cursor and a crosshair. The cursor is user defined,
 * while the crosshair is fixed. The location of either may be set or
 * queried. Only one is active at a time. The methods that active one
 * also deactivative the other. The MCU is able to trigger I/O interrupts
 * when one of the following occurrs: button up/down or key press. The
 * method waitOnInterrupt blocks until an interrupt occurs and returns
 * the reason. Key presses may be retrieved at any time. The key code
 * returned is keyboard specific. 
 *
 */
#ifndef INCMousehh
#define INCMousehh

#include "vxWorks.hh"
#include "semLib.hh"
#include "taskLib.hh"
#include "memLib.hh"

#include "point.h"

/* Private Mouse info. */
#ifdef MOUSE_CLASS
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#endif

#include "MouseP.hh"

static const int KBD_RESET = 0xff;
// This does not follow current version of Tadpole doc.
// static const int BUTTON_UP = 0x2;
// static const int BUTTON_DOWN = 0x4;
static const unsigned char BUTTON_DOWN = 0x2;
static const unsigned char BUTTON_UP = 0x4;
static const unsigned char MOUSE_MOTION = 0x1;
static const unsigned char KBD_INT_MASK = 0x1;

// These signal what buttons are depressed.
static const int RBUTTON_DOWN = 1;
static const int MBUTTON_DOWN = 2;
static const int LBUTTON_DOWN = 4;

// These are the causes of an MCU interrupt.
static const int MOUSE_INT = 1;
static const int KBD_INT =   2;
static const int DUART_INT = 3;
static const int PPORT_INT = 4;

struct keyEvent {
    long time;			/* Time of event. */
    unsigned char keyCode;	/* The actual key pressed. */
    unsigned char padding;	/* Padding to put us on a 2 byte border. */
};

class Mouse {
  private:
    // Base addresses of functional parts.
    void *mouseBase;
    void *cursorBase;
    void *crosshairBase;
    void *kbdBase;

    long *cursorImage;		// Cursor image location.

    unsigned char *interruptMCU; // Hit this to interrupt MCU;
    unsigned char *mcuCmd;	 // MCU command register.
    unsigned char *buttonState;	    // Button state.
    unsigned char *kbdHead;	    // Head of kbd queue.
    unsigned char *kbdTail;	    // It's tail.

    keyEvent *kbdQueue;

    unsigned short *curX;	// Current X postion.

    SEM_ID accessSem;		// Controls access to methods.
    
    MouseISRData isrData;	// Needed to pass data to/from the ISR.
    

  public:
    Mouse(void *addr, Point firstXY, long *cursorBits, int vector);

    /* Cursor methods. */
    void enableCursor(void);

    void setCursorXY(Point newLocation);

    void homeCursor(void) {
	Point x;
	
	x.x = 0;
	x.y = 0;

	setCursorXY(x);
    }

    void setCursorImage(long *cursorbits);

    /* Crosshair only methods. */
    void enableCrosshair(void);

    void setCrosshairXY(Point newLocation);

    void homeCrosshair(void) {
	Point x;
	
	x.x = 0;
	x.y = 0;

	setCrosshairXY(x);
    }

    /* Keyboard methods */
    int getNextKey(void);

    void sendKeyboardCmd(int cmd);

    int keyboardFull(void) {
	if (*kbdTail == *kbdHead + 1)
	  return(1);
	else
	  return(0);
    }
    
    int keyboardEmpty(void) {
	if (*kbdTail == *kbdHead)
	  return(1);
	else
	  return(0);
    }

    /* Common methods. */
    Point getXY(void);

    int getButtonState(void) {
	return(*buttonState);
    }

    int waitOnInterrupt(void);

    void disableInterrupts(int mask);

    void enableInterrupts(int mask);

    ~Mouse(void) {
	*isrData.clearMe = 0xff;
    }
};
    
    
#endif INCMousehh
