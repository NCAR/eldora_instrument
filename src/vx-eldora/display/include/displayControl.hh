/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 displayControl.hh
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
 *        
 */
#ifndef INCdisplayControlhh
#define INCdisplayControlhh

extern "C" {
#include "DisplayRpc.h"
#include "taskLib.h"
#include "Radial.h"

void displayControl(char *,int);
};

#ifndef CONTROL_SCOPE
#define CONTROL_SCOPE extern
#endif

CONTROL_SCOPE int DisplayStatus = STARTING;

CONTROL_SCOPE int getDataTid = 0;
CONTROL_SCOPE int drawDatatid = 0;

CONTROL_SCOPE Radial *R1;
CONTROL_SCOPE Radial *R2;

CONTROL_SCOPE Mouse *Rodent;

CONTROL_SCOPE GraphicController *Gbd;

enum INIT_ACTIONS {
    KILL,
    START,
    SPAWN,
};

extern void initDisplayGlobals(INIT_ACTIONS,DisplayParams *);

#endif INCdisplayControlhh
