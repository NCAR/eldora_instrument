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
 * Revision 1.1  90/12/04  10:19:29  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This is the code for the constructor method for the Tadpole
 * AGC graphics controller. It initializes the window engines, qpdms,
 * MCU, RAMDAC and the video timing. The latter is fixed to suit our
 * display monitors. Note that we are manually clearing memory - don't
 * know why this is needed, but otherwise displays look aweful!
 *
 */
#define GRAPHIC_CTLR_CLASS
#define GRAPHIC_CTLR_PRIVATE
#include "GraphicController.hh"

#include "taskLib.hh"

GraphicController::GraphicController(FAST void *addr, FAST unsigned short
				     screenWidth, FAST unsigned short
				     screenHeight, FAST unsigned short
				     memWidth, FAST unsigned char vector)
{
    FAST WindowRegisters *wptr = windows;

    FAST int j = 4;
    FAST int brs = WDW0_BRS;

// Set up for a max of 4 8-bit windows. 
    for (FAST int i = 0; i < j; i++, brs += 0x10, wptr++)
      {
	  wptr->base = (long *)(addr + (TS_REG_OFFSET | brs));
	  wptr->scrSize = wptr->base + 1;
	  wptr->offsets = wptr->scrSize + 1;
	  wptr->priority = (unsigned short *)(wptr->offsets + 1);
	  wptr->zoom = wptr->priority + 1;
      }

    j = 8;
    FAST int *ones = ONE_BIT_BASES;
    FAST long *onesBase = oneBitWindows;

    for (i = 0; i < j; i++, ones++, onesBase++)
      *onesBase = (long)addr + *ones;

    baseAddr = addr;

    gcr = (unsigned short *)(addr + GCR_OFFSET);
    fifoFlags = (unsigned char *)(addr + GCR_OFFSET + 1);
    maskReg = (unsigned char *)(addr + MASK_OFFSET);
    vectorReg = (unsigned char *)(addr + VECTOR_OFFSET);

    qpdmFifo = addr + QPDM_FIFO;

    accessSem = semCreate();	// Create synch. semaphore and
    semGive(accessSem);		// make sure it's available.

// Start of initialization.

    volatile FAST unsigned short *init = gcr;

// This MUST be done now - if you delay the last 3 writes to later you
// cannot see the display memory!
    *init |= 0x1000;
    *init |= 0x0800;
    *init |= 0x0400;
    *init |= 0x2000;

    taskDelay(1);

    *init |= ~0x1000; 
    taskDelay(1);
    *init |= 0x1000;		// Release clocks.

    *init = ~0x1800;
    taskDelay(1);
    *init = 0x1800;		// Release QPDMs.

    *init = ~0x1c00;
    taskDelay(1);
    *init = 0x1c00;		// Release window engines.

    *init = ~0x3c00;
    taskDelay(1);
    *init = 0x3c06;		// Release MCU and set level 3 interrupts.

    taskDelay(1);

    *maskReg = 0;		// Write in all planes.
    
    *vectorReg = vector & 0xf0;	// We can only set the high nibble.

// This is the display initialization.
    init = (unsigned short *)(addr + TS_REG_OFFSET + 0xc0);

    *init++ = VSYN;
    *init++ = VBSP;
    *init++ = screenHeight;
    *init++ = VFP;
    *init++ = VINIT;
    *init++ = HSYN;
    *init++ = HBSP;
    *init++ = screenWidth;
    *init++ = HFP;
    init++;			// There's a hole in the memory here.
    *init++ = HBLK;

// This is the device initialization.
    *init++ = 0xff;		// Allow active data.

    if (memWidth == 2048)
      {
	  *init++ = 0x23;	// Memory addr. mapping.
	  *init++ = 1;		// Multiplexed addr. width.
      }
    else
      {
	  *init++ = 0xc;
	  *init++ = 2;
      }
    
    *init++ = 0x14;		// VRAM parameters.

    *init = 0x3f;		// Display parameters.

// This is the QPDM initialization.
    init = (unsigned short *)(QPDM_WRITE_ADDR + addr);
    FAST unsigned short *value = (unsigned short *)(addr + QPDM_WRITE_REG);

    *init = 0x1b;		// Reset QPDMs.
    *value = 0;

    *init = 1;			// X start.
    *value = 0;
    
    *init = 2;			// Y start;
    *value = 0;

    *init = 3;			// X end;
    *value = 0x7ff;
    
    *init = 4;			// Y end.
    *value = 0x7ff;

    *init = 0x1c;		// Video timing off.
    *value = 0;

    *init = 0x1d;		// Video refresh off.
    *value = 0;

    *init = 0x17;		// Memory mode - size dependent.
    if (memWidth == 2048)
      *value = 0x38;
    else
      *value = 0x8;

    *init = 0x18;		// Refresh rate.
    *value = 0;

    *init = 0x1a;		// Interrupt sources.
    *value = 0;

    *init = 0x1e;		// Interrupt acknowledge.
    *value = 0x3ff;

// Now set up the RAMDAC to the correct multiplexing, cursor overlay
// items and the read mask.
    FAST unsigned char *ramdacAddr = (unsigned char *)(addr + RAMDAC_ADDR);
    FAST unsigned char *ramdacCtrl = (unsigned char *)(addr + RAMDAC_CONTROL);

    *ramdacAddr = 6;		// Set control for 4:1 mux, nonblinking,
    *ramdacCtrl = 0x42;		// cursor on.

    *ramdacAddr = 5;		// Blink mask - not used.
    *ramdacCtrl = 0;

    *ramdacAddr = 4;		// Read mask - use all bits.
    *ramdacCtrl = 0xff;

    *ramdacAddr = 7;		// Test register - unused.
    *ramdacCtrl = 0;

// Now initialize the RAMDAC with the default color map.
    defaultColorMap();

// Now disable all windows.

    undisplay(0);
    undisplay(1);
    undisplay(2);
    undisplay(3);

    priority(0,0);
    priority(1,0);
    priority(2,0);
    priority(3,0);

// As the last item, enable the QPDM external fifo and set their
// postions, stack, etc.
    FAST unsigned short *qpdm1 = (unsigned short *)(addr + QPDM_FIFO - 0x2000);
    FAST unsigned short *qpdm2 = (unsigned short *)(addr + QPDM_FIFO - 0x1000);

    *qpdm1 = SET_QPDM_POS | 0x80;
    *qpdm2 = SET_QPDM_POS | 0x480;

    FAST unsigned char *gcr2 = fifoFlags;
    volatile FAST unsigned short *fifo = qpdmFifo;

    *gcr |= 0x100;

    *fifo = SET_SCALE;		// Scale factor of 1.
    *fifo = 0;
    *fifo = 0;
    *fifo = 0;

    *fifo = SET_STACK;		// Shouldn't really use stack.
    *fifo = 0x7f0;
    *fifo = 0;
    *fifo = 0x7f0;

    *fifo = CONTROL_CLIP;	// No clipping.

    *fifo = CONTROL_PICK;	// No picking.

    *fifo = DEF_PEL;		// No PEL specified.
    *fifo = 0;

    *fifo = SET_ACT_BITS;	// Use all bits.
    *fifo = 0xff;

    *fifo = SET_LISTEN_BITS;	// All bits used in color/graphic ops.
    *fifo = 0;

    *fifo = SET_COLOR;		// Set default color to black.
    *fifo = 0;
    lastColor = 0;

    lastQpdmOp = G_SET;		// Set default qpdm operation to set.

// Forcably clear video memory. I don't know why, but if this isn't
// done the displayed image has strange speckles in it!
    FAST long *ptr = addr;

    FAST int k = 4096;
    FAST int m = 4096 / sizeof(long);
    FAST long color = (BLACK << 24) | (BLACK << 16) | (BLACK << 8) | BLACK;

    for (i = 0; i < k; i++)
      {
	  for (FAST int l = 0; l < m; l++)
	    *ptr++ = color;
      }
}
