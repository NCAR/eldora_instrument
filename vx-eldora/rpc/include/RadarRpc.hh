/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RadarRpc.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.8  1994/09/07  14:35:01  thor
 * Renamed sendcommand.
 *
 * Revision 1.7  1994/09/06  16:09:47  thor
 * Updated to use TLIRPC.
 *
 * Revision 1.6  1992/08/12  20:12:53  thor
 * Added extern "C" around RadarGbls.h.
 *
 * Revision 1.5  1992/08/06  19:44:02  thor
 * Moved structures to RadarGbls.h.
 *
 * Revision 1.4  1992/07/01  14:59:09  thor
 * Added more C++ protection.
 *
 * Revision 1.3  1992/06/30  14:48:14  thor
 * Fixed typos.
 *
 * Revision 1.2  1992/06/26  18:55:31  thor
 * Added DC offset points variable.
 *
 * Revision 1.1  1992/06/25  17:39:04  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCRadarRpchh
#define INCRadarRpchh

#ifdef OK_RPC

#ifdef UNIX
#ifndef FAST
#define FAST register
#endif
#else // UNIX

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "vxWorks.h"
#ifdef __cplusplus
};
#endif // __cplusplus
#endif // UNIX

#include <string.h>
#include <rpc/rpc.h>

#endif // OK_RPC

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "RadarGbls.h"
#ifdef __cplusplus
};
#endif // __cplusplus

#ifdef OK_RPC

#include "GblCommands.h"
#include "LclCommands.h"

bool_t xdr_RadarCommand(XDR *, RadarCommand *);
bool_t xdr_RadarStatus(XDR *, RadarStatus *);

#define RadarControl ((u_long)0x30000400)
#define RadarCtrlVers ((u_long)1)
#define SendRadarCommand ((u_long)1)
#define GetRadarStatus ((u_long)2)

extern struct RadarStatus *sendradarcommand_1(struct RadarCommand *, CLIENT *);
extern struct RadarStatus *getradarstatus_1(void *, CLIENT *);
extern struct RadarStatus *sendradarcommand_1_svc(struct RadarCommand *,
					 struct svc_req *);
extern struct RadarStatus *getradarstatus_1_svc(void *, struct svc_req *);
extern int radarcontrol_1_freeresult(SVCXPRT *, xdrproc_t, caddr_t);
extern int StartRadarCtrl(void);

#else // OK_RPC
program RadarControl {
    version RadarCtrlVers {
	struct RadarStatus SendRadarCommand(struct RadarCommand) = 1;
	struct RadarStatus GetRadarStatus(void) = 2;
    } = 1;
} = 0x30000400;
#endif // OK_RPC

#endif // INCRadarRpchh

