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
#include <rpc/rpc.h>
#ifndef FAST
#define FAST register
#endif
#else // UNIX

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "vxWorks.h"
#include "string.h"
#include "rpc/rpc.h"
#ifdef __cplusplus
};
#endif // __cplusplus
#endif // UNIX

#endif // OK_RPC

#include "RadarGbls.h"

#ifdef OK_RPC

#include "GblCommands.h"
#include "LclCommands.h"

bool_t xdr_RadarCommand(XDR *, RadarCommand *);
bool_t xdr_RadarStatus(XDR *, RadarStatus *);

#define RadarControl ((u_long)0x30000400)
#define RadarCtrlVers ((u_long)1)
#define SendCommand ((u_long)1)
#define GetRadarStatus ((u_long)2)

#ifdef CLIENT_SIDE
extern struct RadarStatus *sendcommand_1(struct RadarCommand *, CLIENT *);
extern struct RadarStatus *getradarstatus_1(void *, CLIENT *);
#else // CLIENT_SIDE
extern struct RadarStatus *sendcommand_1(struct RadarCommand *,
					 struct svc_req *);
extern struct RadarStatus *getradarstatus_1(void *, struct svc_req *);
extern int StartRadarCtrl(void);
#endif // CLIENT_SIDE

#else // OK_RPC
program RadarControl {
    version RadarCtrlVers {
	struct RadarStatus SendCommand(struct RadarCommand) = 1;
	struct RadarStatus GetRadarStatus(void) = 2;
    } = 1;
} = 0x30000400;
#endif // OK_RPC

#endif // INCRadarRpchh

