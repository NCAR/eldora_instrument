/*
 *	$Id$
 *
 *	Module:		 MouseP.h
 *	Original Author: Richard E. k. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  90/12/04  10:23:28  thor
 * Initial revision
 * 
 *
 *
 * description:
 *      These are items that are only required by the Mouse class
 * methods. Since they are not needed by users, they are hidden here.
 * The keyEvent structure defines the data object the MCU builds for
 * each key press.
 *
 */
#ifndef INCMousePh
#define INCMousePh

/* These are the offsets from the board's base to critical MCU */
/* addresses. */
static const int MOUSE_OFFSET = 0x1000002;
static const int CURSOR_OFFSET = 0x100001a;
static const int CROSSHAIR_OFFSET = 0x1000224;
static const int KBD_OFFSET = 0x1000230;
static const int CURSOR_IMAGE_OFFSET = 0x1000024;
static const int MCU_INT_OFFSET = 0x10003ff;
static const int MCU_CMD_OFFSET = 0x10003fd;
static const int WHY_OFFSET = 0x10003fe;
static const int BUTTON_OFFSET = 0x100000a;
static const int KBD_HEAD_OFFSET = 0x1000290;
static const int KBD_QUEUE_OFFSET = KBD_OFFSET;
static const int CURX_OFFSET = 0x1000006;

/* These are offsets within a functional area. For example, */
/* KBD_CMD_OFF is added to kbdBase to get the kbd command register. */
static const int MOUSE_INT_OFF = 0x9;
static const int CROSS_WIDTH_OFF = 0xa;
static const int KBD_CMD_OFF = 0x62;
static const int CURSOR_SET_OFF = 0x6;
static const int CROSSHAIR_SET_OFF = 0x6;

/* Misc. constants used for setting up the AGC. */
//static const int X_SLEW = 397;	/* See AGC manual. */
//static const int Y_SLEW = 4;
//static const int X_SLEW = 392;	/* See AGC manual. */
static const int X_SLEW = 380;	/* See AGC manual. */
static const int Y_SLEW = -4;
static const int XGEAR = 4;
static const int YGEAR = 4;
static const int X_HI_GEAR = 20;
static const int Y_HI_GEAR = 20;
static const int CURSOR_SIZE = 512;
static const int CURSOR_ENABLE = 0x3;
static const int CROSSHAIR_ENABLE = 0x3;
static const int CROSS_WIDTH = 4;
static const int CURSOR_PARAM_CHANGE = 0x3;
static const int CURSOR_XY_CHANGE = 0x4;
static const int CROSSHAIR_PARAM_CHANGE = 0x5;
static const int CROSSHAIR_XY_CHANGE = 0x6;
static const int SEND_KBD_CMD = 0x7;
static const int NO_KEY_EVENT = 0xffff0000;
static const int KEY_BUF_SIZE = 16;

/* ISR structure - needed to pass both semaphore and AGC board address. */
struct mouseISRData {
    SEM_ID sem;
    unsigned char *clearMe;
    int interruptReason;
};

typedef struct mouseISRData MouseISRData;

#endif INCMousePh

