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
#ifdef MVME133
extern void RadialLoop(Task &self, GraphicController *agc);
extern void HorizLoop(Task &self, GraphicController *agc);
extern void VertLoop(Task &self, GraphicController *agc);
extern void VertLoop(Task &self, GraphicController *agc, Pipe &pipe);

extern void RadialMouse(Radial *);
extern void HorizMouse(HorizDisplay *);
extern void VertMouse(Vertical *);

#endif // INCGeDrawhh

