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
#include "rpc/rpc.h"
#else
#include <rpc/rpc.h>
#include <rpc/types.h>
#endif /* UNIX */

#ifndef FAST
#define FAST register
#endif /* FAST */

#endif /* OK_RPC */

enum LOGFILES {
    EVENTLOG,
    ERRORLOG
  };

struct LOG {
    enum LOGFILES file;
    char message[81];
};

#ifdef OK_RPC

typedef enum LOGFILES LOGFILES;
bool_t xdr_LOGFILES(XDR *xdrs, LOGFILES *objp);

typedef struct LOG LOG;
bool_t xdr_LOG(XDR *xdrs, LOG *objp);

#define Logger ((u_long)0x30002000)
#define LoggerVers ((u_long)1)
#define LogMessage ((u_long)1)

#ifdef CLIENT_SIDE
extern void logmessage_1(LOG *argp);
extern void loggerError(char *message);
extern void loggerEvent(char *message);
extern int loggerInit(char *host);

#define LOGGER_PRI 2

#ifndef LOGGER_SCOPE
#define LOGGER_SCOPE extern
#endif

LOGGER_SCOPE STATUS LoggerError;

#else
extern void *logmessage_1(LOG *argp, struct svc_req *);
extern void logger_1(struct svc_req *rqstp, SVCXPRT *transp);

#endif /* CLIENT_SIDE */

#else

program Logger {
    version LoggerVers {
	void LogMessage(struct LOG) = 1;
    } = 1;
} = 0x30002000;

#endif /* OK_RPC */

#endif /* INCloggerh */

