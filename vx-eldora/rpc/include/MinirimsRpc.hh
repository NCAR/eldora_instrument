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
#include <iostream.h>

#endif /* OK_RPC */

struct MinirimsCommand {
  u_long cmd;
};

#ifdef OK_RPC

typedef struct MinirimsCommand MinirimsCommand;

extern bool_t xdr_MinirimsCommand(XDR *, MinirimsCommand *);

#define MinirimsControl ((unsigned long)(0x30000300))
#define MinirimsCtrlVers ((unsigned long)(1))

#define sendMinirimsCommand ((unsigned long)(1))
extern  int * sendminirimscommand_1(struct MinirimsCommand *, CLIENT *);
extern  int * sendminirimscommand_1_svc(struct MinirimsCommand *,
					struct svc_req *);
extern void MinirimsRpcInit();
#else
#ifdef RPCGEN
program MinirimsControl {
  version MinirimsCtrlVers {
    int sendMinirimsCommand(struct MinirimsCommand) = 1;
  } = 1;
} = 0x30000300;
#endif /* RPCGEN */
#endif /* OK_RPC */

#endif // _MINIRIMSRPC_HH
