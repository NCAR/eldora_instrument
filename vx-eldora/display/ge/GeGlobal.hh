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


#ifndef GE_SCOPE
#define GE_SCOPE extern
#endif // GE_SCOPE

GE_SCOPE DispStatus *GeStatus;
GE_SCOPE DispStatus *AlarmStatus;
GE_SCOPE Header *Hdr;
GE_SCOPE GeAlarm *Alarm;
GE_SCOPE int system;

// Functions known locally.
extern "C" {
void GeStart(char *server, int system);
};

extern void AlarmLoop(char *server, int sys);

#endif // INCGeGlobalhh 
