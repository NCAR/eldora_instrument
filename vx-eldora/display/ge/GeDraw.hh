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
#include "HorizDisplay.hh"
#include "Vertical.hh"

// Uniprocessor.
#include "Horiz.hh"
#include "Pipe.hh"

#ifdef MVME133
static const int AGC_ADDR                 = 0xf0000000;
#else
static const int AGC_ADDR                 = 0xa0000000;
#endif // MVME133

static const int MCPL_ADDR                = 0x20000000;

static const unsigned short AGC_WIDTH     = 1280;
static const unsigned short AGC_HEIGHT    = 1024;
static const unsigned short AGC_MEM_WIDTH = 2048;
static const unsigned short AGC_VECTOR    = 250;

static const unsigned int NEW_DATA_FLAG = 0x1000;
static const unsigned int DESTROY_SELF  = 0x2000;
static const unsigned int MOUSE_FLAG    = 0x4000;
static const unsigned int TMO_FLAG      = 0x8000;

static const unsigned int waitMask = (STOP | START | RELOAD | FORWARD_RADIAL |
				      FORWARD_HORIZ | FORWARD_VERT |
				      AFT_RADIAL | AFT_HORIZ | AFT_VERT |
				      RESTART| NEW_DATA_FLAG | DESTROY_SELF | 
				      MOUSE_FLAG | LOAD_ONLY);

static const float MAX_RECT = 30000.0;

static const float EldoraBeam = 2.0; // In degrees.

static const int DDP_VECTOR = 243;

static const int DDP_PRI = 50;

#ifdef MVME133
static const int DDP_ADDR = 0x02000000;
#else
static const int DDP_ADDR = 0x40200000;
#endif // MVME133

extern void RadialLoop(Task &self, GraphicController *agc, Pipe &pipe);
extern void HorizLoop(Task &self, GraphicController *agc, Pipe &pipe);
extern void VertLoop(Task &self, GraphicController *agc, Pipe &pipe);

extern void RadialMouse(Radial *);
extern void HorizMouse(HorizDisplay *);
extern void VertMouse(Vertical *);

#endif // INCGeDrawhh

