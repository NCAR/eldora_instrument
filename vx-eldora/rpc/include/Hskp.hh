/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Hskp.hh
 *	Original Author: Richard E. Neitzel
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
#ifndef INCHskphh
#define INCHskphh

#ifdef OK_RPC

#ifdef UNIX

#ifndef FAST
#define FAST register
#endif // FAST

#include <rpc/rpc.h>

#else // UNIX
extern "C" {
#include "vxWorks.h"
#include "string.h"
#include "rpc/rpc.h"
};
#endif // UNIX

#endif // OK_RPC

#include "HskpCmd.h"
#include "HskpStatus.h"

#ifdef OK_RPC

#define HskpControl ((u_long)0x30000500)
#define HskpCtrlVers ((u_long)1)
#define SendCommand ((u_long)1)
#define GetHskpStatus ((u_long)2)

bool_t xdr_HskpCommand(XDR *, HskpCommand *);
bool_t xdr_HskpStatus(XDR *, HskpStatus *);

#ifdef CLIENT_SIDE
extern struct HskpStatus *sendcommand_1(struct HskpCommand *, CLIENT *);
extern struct HskpStatus *gethskpstatus_1(void *, CLIENT *);
#else // CLIENT_SIDE
extern int HskpRpcInit(void);
extern struct HskpStatus *sendcommand_1(struct HskpCommand *, struct svc_req *);
extern struct HskpStatus *gethskpstatus_1(void *, struct svc_req *);
#endif // CLIENT_SIDE

#else // OK_RPC
program HskpControl {
    version HskpCtrlVers {
	struct HskpStatus SendCommand(struct HskpCommand) = 1;
	struct HskpStatus GetHskpStatus(void) = 2;
    } = 1;
} = 0x30000500;
#endif // OK_RPC
#endif // INCHskphh

