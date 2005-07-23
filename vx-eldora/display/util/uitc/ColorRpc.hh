/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorRpc.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/19  13:50:00  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
#ifndef INCColorRpchh
#define INCColorRpchh

#define MAX_COLORS_SET 31

struct ColorCmd {
    int index;
    long colors[256];
};

#ifdef OK_RPC

#ifdef UNIX

#include <rpc/rpc.h>
#include <rpc/types.h>

#define FAST register

#else

#ifdef __cplusplus
extern "C" {
#endif
#include "vxWorks.h"
#include "rpc/rpc.h"
#ifdef __cplusplus
};
#endif

#endif // UNIX

typedef struct ColorCmd ColorCmd;

#ifdef __cplusplus
extern "C" {
#endif
bool_t xdr_ColorCmd(XDR*, ColorCmd *);
#ifdef __cplusplus
};
#endif

#define MAX_COLORS_SET 31

static const long DO_ALL  = -1;
static const long USE_DEF = -2;

#define ColorSetProg ((u_long)0x30001000)
#define ColorSetVers ((u_long)1)
#define ColorSetProc ((u_long)1)

#ifdef CLIENT_SIDE
void *colorsetproc_1(ColorCmd *argp, CLIENT *clnt);
#else
#ifdef __cplusplus
extern "C" {
#endif
int ColorRpcInit(void);
void *colorsetproc_1(void *, struct svc_req *);
#ifdef __cplusplus
};
#endif
#endif // CLIENT_SIDE
#else

program ColorSetProg {
    version ColorSetVers {
	void ColorSetProc(struct ColorCmd) = 1;
    } = 1;
} = 0x30001000;

#endif // OK_RPC
#endif // INCColorRpchh

