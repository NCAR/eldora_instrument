/*
 *	$Id$
 *
 *	Module:		 DispAlarm.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/02  20:44:10  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1991/09/18  15:30:17  thor
 * Fixed includes.
 *
 * Revision 1.1  1991/09/16  17:39:07  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCDispAlarmh
#define INCDispAlarmh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>

#ifndef FAST
#define FAST register
#endif /* FAST */

#else
extern "C" {
#include "vxWorks.h"
#include "rpc/rpc.h"
#include "rpc/pmap_clnt.h"
};
#endif /* UNIX */

#endif /* OK_RPC */

#include "DisplayRpc.h"

#ifdef OK_RPC

#define DispAlarm ((u_long)0x30000201)
#define DispAlarmVers ((u_long)1)
#define SendAlarm ((u_long)1)

#ifdef WRS_SIDE
extern void *sendalarm_1(DispStatus *, CLIENT *);
#else
extern int DispAlarmInit(void);
extern void *sendalarm_1(DispStatus *, struct svc_req *);
#endif /* WRS_SIDE */

#else

program DispAlarm {
    version DispAlarmVers {
	void SendAlarm(struct DispStatus) = 1;
    } = 1;
} = 0x30000201;

#endif /* OK_RPC */

#endif /* INCDispAlarmh */

