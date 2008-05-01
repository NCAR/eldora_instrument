#if 0 /* For rpcgen's sake. */
//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          MinirimsRpc.hh
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1996/07/18  20:06:28  thor
// Initial revision
//
//
//
// description:
//        
//
#endif
#ifndef _MINIRIMSRPC_HH
#define _MINIRIMSRPC_HH

#ifdef OK_RPC
#ifdef UNIX
#ifndef FAST
#define FAST register
#endif /* FAST */
#else
#include "vxWorks.h"
#include <string.h>
#endif /* UNIX */

#include <rpc/rpc.h>
#include <stdlib.h>

#endif /* OK_RPC */

struct MinirimsCommand {
  u_long cmd;
};

struct MinirimsStatus {
  u_long status;
};

#ifdef OK_RPC

typedef struct MinirimsCommand MinirimsCommand;
typedef struct MinirimsStatus MinirimsStatus;

extern bool_t xdr_MinirimsCommand(XDR *, MinirimsCommand *);
extern  bool_t xdr_MinirimsStatus(XDR *, MinirimsStatus*);

#define MinirimsControl ((unsigned long)(0x30000300))
#define MinirimsCtrlVers ((unsigned long)(1))

#define sendMinirimsCommand ((unsigned long)(1))
extern  int * sendminirimscommand_1(struct MinirimsCommand *, CLIENT *);

#define statusMinirims ((unsigned long)(2))
extern  struct MinirimsStatus * statusminirims_1(void *, CLIENT *);

#ifdef __cplusplus
extern "C" {
#endif
extern  int * sendminirimscommand_1_svc(struct MinirimsCommand *,
					struct svc_req *);
extern  struct MinirimsStatus * statusminirims_1_svc(void *, struct svc_req *);
extern int minirimscontrol_1_freeresult(SVCXPRT *, xdrproc_t, caddr_t);
extern void MinirimsRpcInit();
#ifdef __cplusplus
};
#endif

#else
#ifdef RPCGEN
program MinirimsControl {
  version MinirimsCtrlVers {
    int sendMinirimsCommand(struct MinirimsCommand) = 1;
    struct MinirimsStatus(void) = 2;
  } = 1;
} = 0x30000300;
#endif /* RPCGEN */
#endif /* OK_RPC */

#endif // _MINIRIMSRPC_HH
