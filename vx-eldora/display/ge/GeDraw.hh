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
#include "Pipe.hh"
static const int AGC_ADDR                 = 0xa0000000;
#endif // MVME133

static const int MCPL_ADDR                = 0x20000000;

static const unsigned int NEW_DATA_FLAG = 0x1000;
static const unsigned int DESTROY_SELF  = 0x2000;
static const unsigned int MOUSE_FLAG    = 0x4000;
				      RESTART);

static const unsigned int NEW_DATA_FLAG = 0x1000;
static const unsigned int DESTROY_SELF  = 0x2000;
static const unsigned int MOUSE_FLAG    = 0x4000;
static const unsigned int waitMask = (STOP | START | RELOAD | FORWARD_RADIAL |
				      FORWARD_HORIZ | FORWARD_VERT |
				      AFT_RADIAL | AFT_HORIZ | AFT_VERT |
				      RESTART| NEW_DATA_FLAG | DESTROY_SELF | 
				      MOUSE_FLAG);

static const float MAX_RECT = 30000.0;

static const float EldoraBeam = 2.0; // In degrees.
static const int DDP_VECTOR = 243;
#ifdef MVME133
static const int DDP_ADDR = 0x02000000;
#else
static const int DDP_ADDR = 0x40200000;
#endif // MVME133

extern void RadialLoop(Task &self, GraphicController *agc, Pipe &pipe);
extern void VertLoop(Task &self, GraphicController *agc, Pipe &pipe);

extern void RadialMouse(Radial *);
extern void HorizMouse(HorizDisplay *);
extern void VertMouse(Vertical *);

#endif // INCGeDrawhh

