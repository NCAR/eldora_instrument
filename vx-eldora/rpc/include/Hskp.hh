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
 * Revision 1.1  1992/07/13  20:19:00  thor
 * Initial revision
 *
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

#else // UNIX
#include <vxWorks.h>
#include <string.h>
#endif // UNIX

#include <rpc/rpc.h>

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

extern struct HskpStatus *sendcommand_1(struct HskpCommand *, CLIENT *);
extern struct HskpStatus *gethskpstatus_1(void *, CLIENT *);
extern struct HskpStatus *sendcommand_1_svc(struct HskpCommand *,
                                            struct svc_req *);
extern struct HskpStatus *gethskpstatus_1_svc(void *, struct svc_req *);

#else // OK_RPC
program HskpControl {
    version HskpCtrlVers {
	struct HskpStatus SendCommand(struct HskpCommand) = 1;
	struct HskpStatus GetHskpStatus(void) = 2;
    } = 1;
} = 0x30000500;
#endif // OK_RPC
#endif // INCHskphh

