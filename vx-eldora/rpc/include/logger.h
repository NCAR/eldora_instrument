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
 * Revision 2.2  1994/09/07  16:43:33  thor
 * Updated to the new TLIRPC.
 *
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

#ifndef UNIX
#include "vxWorks.h"
#endif /* UNIX */

#ifndef FAST
#define FAST register
#endif /* FAST */

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

typedef enum LOGSOURCE LOGSOURCE;
typedef struct LOGMSG LOGMSG;
static const int loggerPort = 3001;

#ifdef CLIENT_SID

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */E

extern void loggerEvent(char *message, int *ip, int num);
extern int loggerInit(int src);

#ifdef __cplusplus
};
#endif /* cplusplus */

#define LOGGER_PRI 2

#ifndef LOGGER_SCOPE
#define LOGGER_SCOPE extern
#endif

LOGGER_SCOPE STATUS LoggerError;

#endif /* CLIENT_SIDE */

#endif /* INCloggerh */




