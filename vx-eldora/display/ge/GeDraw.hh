/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeDraw.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.4  1994/09/23  15:03:57  thor
 * Removed unused include file.
 *
 * Revision 2.3  1994/04/08  20:59:28  thor
 * Redid flags & wait masks. Dropped multiple loop tasks for 1 task.
 *
 * Revision 2.2  1993/09/28  13:02:48  thor
 * Changed flag values to avoid conflicts with others, added dual display supt.
 *
 * Revision 2.1  1993/08/20  17:14:30  thor
 * Changed AGC address to work under MMU.
 *
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.10  1992/10/09  14:55:25  thor
 * Added LOAD_ONLY support.
 *
 * Revision 1.9  1992/10/02  20:45:41  thor
 * Added base address of MCPL.
 *
 * Revision 1.8  1992/01/27  18:33:51  thor
 * Changed addresses for AGC & DDP.
 *
 * Revision 1.7  1992/01/08  16:20:04  thor
 * Added code for timeout change.
 *
 * Revision 1.6  1991/12/18  16:51:09  thor
 * Added more flags to the list.
 *
 * Revision 1.5  1991/11/01  20:01:11  thor
 * Added new constants and changed prototypes for drawing tasks.
 *
 * Revision 1.4  1991/10/30  17:59:08  thor
 * Added include for horiz. display, consts for beam width & horiz/vert
 * grid size.
 *
 * Revision 1.3  1991/10/23  20:38:07  thor
 * Added Mouse support.
 *
 * Revision 1.2  1991/10/17  16:49:15  thor
 * Added NEW_DATA_FLAG & DESTROY_SELF flags.
 *
 * Revision 1.1  1991/10/14  19:22:08  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCGeDrawhh
#define INCGeDrawhh

#include "Radial.hh"
#include "Vert.hh"
#include "Dual.hh"
#include "Raw.hh"

// Uniprocessor.
#include "Horiz.hh"
#include "Pipe.hh"

#ifdef MVME133
static const int AGC_ADDR                 = 0xf0000000;
#else
static const int AGC_ADDR                 = 0x80000000;
#endif // MVME133

static const int MCPL_ADDR                = 0x20000000;

static const unsigned short AGC_WIDTH     = 1280;
static const unsigned short AGC_HEIGHT    = 1024;
static const unsigned short AGC_MEM_WIDTH = 2048;
static const unsigned short AGC_VECTOR    = 250;

static const unsigned int NEW_DATA_FLAG = 0x10000000;
static const unsigned int UNDISPLAY     = 0x20000000;
static const unsigned int SHOW_FORWARD  = 0x00100000;
static const unsigned int SHOW_AFT      = 0x00200000;
static const unsigned int TMO_FLAG      = 0x08000000;

static const unsigned int mainMask = (STOP | START | RELOAD | FORWARD_RADIAL |
				      FORWARD_HORIZ | FORWARD_VERT |
				      AFT_RADIAL | AFT_HORIZ | AFT_VERT |
				      FORWARD_DUAL | AFT_DUAL |
                                      FORWARD_RAW | AFT_RAW | 
				      RESTART | NEW_DATA_FLAG | UNDISPLAY | 
				      LOAD_ONLY | REBOOT);

static const unsigned int waitMask = (STOP | START | LOAD_ONLY | SHOW_FORWARD |
				      SHOW_AFT | NEW_DATA_FLAG | UNDISPLAY);

static const float MAX_RECT = 30000.0;

static const float EldoraBeam = 2.0; // In degrees.

static const int DDP_VECTOR = 243;

static const int DDP_PRI = 50;

#ifdef MVME133
static const int DDP_ADDR = 0x02000000;
#else
static const int DDP_ADDR = 0x40200000;
#endif // MVME133

// This the drawing task.
extern void DisplayLoop(Task &self, Pipe &pipe, SEM_ID stopSem);

#endif // INCGeDrawhh
