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
extern "C" {
#include "DisplayRpc.h"
#include "TapeHeader.h"
};

#include "Header.hh"
#include "GeAlarm.hh"
#include "Task.hh"
#include "Mouse.hh"

#define WINDOW_GBLS_SCOPE
#include "GblRadial.hh"


#ifndef GE_SCOPE
#define GE_SCOPE extern
#endif // GE_SCOPE

GE_SCOPE DispStatus *GeStatus;
GE_SCOPE DispStatus *AlarmStatus;
GE_SCOPE DispCommand *GeCommand;
GE_SCOPE Header *Hdr;
GE_SCOPE GeAlarm *Alarm;
GE_SCOPE int system;

GE_SCOPE Mouse *Rodent;

GE_SCOPE Task *AlarmTask;
GE_SCOPE Task *CtrlTask;
GE_SCOPE Task *DrawingTask;

// Flag masks.
static const unsigned int TASK_STOP   = 0x1;
static const unsigned int TASK_START  = 0x2;
static const unsigned int ATASK_RESET = 0x4;
static const unsigned int ATASK_SIGNAL = 0x8;

// Functions known locally.
extern "C" {
void GeStart(char *server, int system);

// Entry point list.
extern void AlarmLoop(Task &self, char *server, int sys);
extern void RpcLoop(Task &self);
extern void DrawingLoop(Task &self);
};

// Priorities.
static const int DRAWING_PRI = 50;
static const int CTRL_PRI = 3;
static const int ALARM_PRI = 2;
static const int GRAPH_PRI = 51;
static const int MOUSE_PRI = 49;

#endif // INCGeGlobalhh 
