/*
 *	$Id$
 *
 *	Module:		 tapeControl.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/28  19:49:14  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCtapeControlh
#define INCtapeControlh

#ifdef OK_RPC

#ifdef TAE
#include <rpc/rpc.h>
#define FAST register
#else

#ifndef UNIX
#include "vxWorks.h"
#include "rpc/rpc.h"
#include "taskLib.h"
#else
#include <rpc/rpc.h>
#define FAST register
#endif /* UNIX */

#endif /* TAE */

#endif /* OK_RPC */

struct TapeCommand {
    u_long cmd;
    u_long count;
    int unit;
};

struct TapeError {
    int error;
};

struct ScsiError {
    int error;
};

struct TapeStatus {
    u_long status;
    u_long count;
    u_long failures;
    u_long attempts;
    int eot_warning;
    int unit;
    struct TapeError terr;
    struct ScsiError serr;
};

#ifdef OK_RPC

#include "tapeCommands.h"
#include "tapeStatus.h"

typedef struct TapeCommand TapeCommand;
bool_t xdr_TapeCommand(XDR *, TapeCommand *);

typedef struct TapeError TapeError;
bool_t xdr_TapeError(XDR *, TapeError *);

typedef struct ScsiError ScsiError;
bool_t xdr_ScsiError(XDR *, ScsiError *);

typedef struct TapeStatus TapeStatus;
bool_t xdr_TapeStatus(XDR *, TapeStatus *);

#define TapeControl ((u_long)0x30000300)
#define TapeCtrlVers ((u_long)1)
#define SendCommand ((u_long)1)
#define GetTapeStatus ((u_long)2)

#ifdef CLIENT_SIDE
extern struct TapeStatus *sendcommand_1(TapeCommand *, CLIENT *);
extern struct TapeStatus *gettapestatus_1(void *, CLIENT *);
#else
extern struct TapeStatus *sendcommand_1(TapeCommand *, struct svc_req *);
extern struct TapeStatus *gettapestatus_1(void *, struct svc_req *);
#endif

void startControl(void);

#else
program TapeControl {
    version TapeCtrlVers {
	struct TapeStatus SendCommand(struct TapeCommand) = 1;
	struct TapeStatus GetTapeStatus(void) = 2;
    } = 1;
} = 0x30000300;
#endif

#ifdef __GNUC__
static int UNPACKSTATUS(FAST int status, FAST int unit);
static void PACKSTATUS(FAST int status, FAST int *blk, FAST int unit);

inline static int UNPACKSTATUS(FAST int status, FAST int unit)
{
    if (unit == 0)
      return(status & 0xffff);
    else
      return(status >> 16);
}

inline static void PACKSTATUS(FAST int status, FAST int *blk, FAST int unit)
{
    FAST int i = *blk;

    if (unit == 1)
      {
	  status <<= 16;
	  i &= 0xffff;
	  i |= status;
      }
    else
      {
	  i &= 0xffff0000;
	  i |= status;
      }
    *blk = i;
}

#endif /* GNUC */
      
#endif /* INCtapeControlh */

