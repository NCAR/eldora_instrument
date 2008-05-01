/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeGlobal.hh
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
 * Revision 2.3  1994/09/12  18:43:56  thor
 * Removed alarm stuff.
 *
 * Revision 2.2  1994/04/08  21:00:24  thor
 * Added items needed for new design.
 *
 * Revision 2.1  1993/08/20  17:14:30  thor
 * Changed system to dispSystem due to name clash with VxWorks.
 *
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.8  1992/09/01  17:54:07  thor
 * Moved DisplayRpc.h out of extern block.
 *
 * Revision 1.7  1992/06/29  17:32:19  thor
 * Added global Ddp pointer. Increased priority of control loop task.
 *
 * Revision 1.6  1991/12/03  19:26:29  thor
 * Changed priorities so header download works.
 *
 * Revision 1.5  1991/11/01  20:02:01  thor
 * Added new static for time keeping.
 *
 * Revision 1.4  1991/10/23  20:38:35  thor
 * Added Mouse support.
 *
 * Revision 1.3  1991/10/15  15:37:34  thor
 * Added include of globals for radial case.
 *
 * Revision 1.2  1991/10/14  19:20:39  thor
 * Added Task control items.
 *
 * Revision 1.1  1991/10/01  16:15:15  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCGeGlobalhh
#define INCGeGlobalhh

#define OK_RPC
#define WRS_SIDE
#include "DisplayRpc.h"
extern "C" {
#include "TapeHeader.h"
};

#include "Header.hh"
#include "Task.hh"
#include "Mouse.hh"
#include "Display.hh"
#include "Ddp.hh"

#ifndef GE_SCOPE
#define GE_SCOPE extern
#endif // GE_SCOPE

GE_SCOPE DispStatus *GeStatus;
GE_SCOPE DispCommand *GeCommand;
GE_SCOPE Header *Hdr;
GE_SCOPE int dispSystem;
GE_SCOPE Display *display;
GE_SCOPE Mouse *Rodent;

GE_SCOPE Task *CtrlTask;
GE_SCOPE Task *DrawingTask;

GE_SCOPE Beam_Time *LastTime;

GE_SCOPE Ddp *DdpCtrl;

// Flag masks.
static const unsigned int TASK_STOP   = 0x1;
static const unsigned int TASK_START  = 0x2;
static const unsigned int ATASK_RESET = 0x4;
static const unsigned int ATASK_SIGNAL = 0x8;

// Functions known locally.
extern "C" {
void GeStart();

// Entry point list.
extern void SocketLoop(Task &self);
extern void DrawingLoop(Task &self);
extern void FieldMouseEvents(Task &self, Mouse &rodent);
};
void svrTask(int port);

// Priorities.
static const int DRAWING_PRI = 15;
static const int CTRL_PRI = 10;
static const int GRAPH_PRI = 105;
static const int MOUSE_PRI = 40;

#endif // INCGeGlobalhh 
