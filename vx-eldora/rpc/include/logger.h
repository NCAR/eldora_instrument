/*
 *	$Id$
 *
 *	Module:		 logger.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.1  1993/09/10  16:42:56  thor
 * New improved version!
 *
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 1.3  1992/01/16  19:40:57  thor
 * Decreased size of messages to 81 characters & fixed logmessage_1 prototype.
 *
 * Revision 1.2  1992/01/14  19:46:24  thor
 * Added functions to transparently move messages.
 *
 * Revision 1.1  1992/01/14  19:18:20  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCloggerh
#define INCloggerh

#ifdef OK_RPC

#ifndef UNIX
#include "vxWorks.h"
#else
#include <rpc/types.h>

#endif /* UNIX */

#include <rpc/rpc.h>

#ifndef FAST
#define FAST register
#endif /* FAST */

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */
#endif /* OK_RPC */

enum LOGSOURCE {
	FORE_LOG = 0,
	AFT_LOG = 1,
	DISP1_LOG = 2,
	DISP2_LOG = 3,
	RECORD_LOG = 4,
	HSKP_LOG = 5,
};

struct LOGMSG {
	enum LOGSOURCE src;
	char message[81];
	int items[10];
};

#ifdef OK_RPC

typedef enum LOGSOURCE LOGSOURCE;
bool_t xdr_LOGSOURCE(XDR *xdrs, LOGSOURCE *objp);

typedef struct LOGMSG LOGMSG;
bool_t xdr_LOGMSG(XDR *xdrs, LOGMSG *objp);

#define Logger ((u_long)0x30002000)
#define LoggerVers ((u_long)1)
#define LogMessage ((u_long)1)

#ifdef CLIENT_SIDE

extern void logmessage_1(LOGMSG *argp);
extern void loggerEvent(char *message, int *ip, int num);
extern int loggerInit(int src);

#define LOGGER_PRI 2

#ifndef LOGGER_SCOPE
#define LOGGER_SCOPE extern
#endif

LOGGER_SCOPE STATUS LoggerError;

#ifdef __cplusplus
};
#endif /* cplusplus */

#else
extern void *logmessage_1_svc(LOGMSG *argp, struct svc_req *);
extern void logger_1(struct svc_req *rqstp, SVCXPRT *transp);

#endif /* CLIENT_SIDE */

#else

program Logger {
    version LoggerVers {
	void LogMessage(struct LOGMSG) = 1;
    } = 1;
} = 0x30002000;

#endif /* OK_RPC */

#endif /* INCloggerh */




