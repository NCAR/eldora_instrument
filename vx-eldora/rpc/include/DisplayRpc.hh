#if 0 /* For rpcgen's sake. */
//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          DisplayRpc.hh
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 3.1  1994/09/07  13:35:55  thor
// Renamed sendcommand.
//
// Revision 3.0  1994/08/17  16:08:34  thor
// New version to work with Sun's TLI version of RPC.
//
//
//
// description:
//        
//
#endif
#ifndef _DISPLAYRPC_HH
#define _DISPLAYRPC_HH

#ifdef OK_RPC
#ifdef UNIX
#ifndef FAST
#define FAST register
#endif /* FAST */
#else
#include "vxWorks.h"
#endif /* UNIX */

#include <rpc/rpc.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

#endif /* OK_RPC */

struct DispCommand {
    u_long cmd;			/* Current command. */
    u_long numParams;		/* # of parameters to draw. */
    u_long param0;		/* Offset of parameter 0. */
    u_long param1;		/* Offset of parameter 1. */
    u_long param2;		/* Offset of parameter 2. */
    u_long side;		/* Left or right side? */
    u_long radius;		/* Size of radial display in pixels. */
    u_long tmo;			/* Timeout in seconds. */
    u_long userColors;          /* Color table to use. 0 = default.*/
    double max0;		/* Maximum displayed value for p0. */
    double max1;		/* Maximum displayed value for p1. */
    double max2;		/* Maximum displayed value for p2. */
    double min0;		/* Minimum  displayed value for p0. */
    double min1;		/* Minimum  displayed value for p1. */
    double min2;		/* Minimum  displayed value for p2. */
    double distance;		/* Distance to plane or strip in m. */
    double size;		/* Size of region for horizontial. */
    double top;			/* Maximum altitude for vertical. */
    double bottom;		/* Minimum altitude for vertical. */
};

struct DispStatus {
    u_long status;		/* Current status. */
};

#ifdef OK_RPC

#include "DispCommands.h"
#include "DispStatus.h"

typedef struct DispCommand DispCommand;
typedef struct DispStatus DispStatus;

bool_t xdr_DispCommand(XDR *, DispCommand *);
bool_t xdr_DispStatus(XDR *, DispStatus *);

#define DispControl ((u_long)0x30000200)
#define DispCtrlVers ((u_long)1)
#define SendDispCommand ((u_long)1)
#define GetStatus ((u_long)2)

extern struct DispStatus *senddispcommand_1(DispCommand *, CLIENT *);
extern struct DispStatus *getstatus_1(void *, CLIENT *);
extern struct DispStatus *senddispcommand_1_svc(DispCommand *,
                                                struct svc_req *);
extern struct DispStatus *getstatus_1_svc(void *, struct svc_req *);
extern void DispRpcInit(void);

#else
#ifdef RPCGEN
program DispControl {
    version DispCtrlVers {
	struct DispStatus SendDispCommand(struct DispCommand) = 1;
	struct DispStatus GetStatus(void) = 2;
    } = 1;
} = 0x30000200;
#endif /* RPCGEN */
#endif /* OK_RPC */

#endif
