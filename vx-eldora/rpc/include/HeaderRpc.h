/*
 *	$Id$
 *
 *	Module:		 HeaderRpc.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/08/30  16:52:48  thor
 * Added define of startHeader for backwards compatibility.
 *
 * Revision 1.1  1994/08/30  15:16:33  thor
 * Initial revision
 *
 *
 *
 * description:
 * If this is the VxWorks side we need to drag in the header files for
 * Vxworks & the Header class. We also need to declare the global
 * pointer for the currently in use Header object. Naturally this is
 * complicated by the fact that not all of us are using C++, so the C
 * interface header is grabbed if C is being used.
 *
 */
#ifndef _HEADERRPC_H_RPCGEN
#define	_HEADERRPC_H_RPCGEN

#ifdef WRS
#include <vxWorks.h>
#ifndef VXWORKS
#define VXWORKS
#endif
#ifndef HEADERRPC_SCOPE
#define HEADERRPC_SCOPE extern
#endif

#ifdef __cplusplus
#include "Header.hh"
HEADERRPC_SCOPE Header *Hdr;
#else
#include "Header.h"
HEADERRPC_SCOPE HeaderPtr Hdr;
#endif

#endif /* WRS */

#include <rpc/rpc.h>

#ifdef __cplusplus
extern "C" {
#endif


struct headername {
	char name[256];
};
typedef struct headername headername;

#define	HeaderRpc ((unsigned long)(0x30000100))
#define	HeaderRpcVers ((unsigned long)(1))

#if defined(__STDC__) || defined(__cplusplus)
#define	HeaderRpcFunc ((unsigned long)(1))
extern  int * headerrpcfunc_1(struct headername *, CLIENT *);
extern  int * headerrpcfunc_1_svc(struct headername *, struct svc_req *);
extern int headerrpc_1_freeresult(SVCXPRT *, xdrproc_t, caddr_t);
#ifdef VXWORKS
extern void HeaderRpcInit(void);
#define startHeader HeaderRpcInit
#endif
#else /* K&R C */
#define	HeaderRpcFunc ((unsigned long)(1))
extern  int * headerrpcfunc_1();
extern  int * headerrpcfunc_1_svc();
extern int headerrpc_1_freeresult();
#ifdef VXWORKS
extern void HeaderRpcInit();
#define startHeader HeaderRpcInit
#endif
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_headername(XDR *, headername*);

#else /* K&R C */
extern bool_t xdr_headername();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_HEADERRPC_H_RPCGEN */
