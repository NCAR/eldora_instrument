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
 * Revision 1.6  1994/09/02  18:17:41  thor
 * Major rewrite.
 *
 * Revision 1.5  1992/05/07  16:32:46  thor
 * Changed code to handle nmon-gcc compilation.
 *
 * Revision 1.4  1992/01/14  15:21:54  thor
 * Added code for data reduction.
 *
 * Revision 1.3  1991/09/06  16:34:06  thor
 * Fixed (UN)PACKSTATUS to use ulong.
 *
 * Revision 1.2  1991/09/06  16:08:53  thor
 * Changed RPC registration routine name.
 *
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

#ifndef UNIX
#include "vxWorks.h"
#include "taskLib.h"
#else
#define FAST register
#endif /* UNIX */

#include <rpc/rpc.h>

#endif /* OK_RPC */

struct TapeCommand {
    u_long cmd;			/* Command */
};

struct TapeStatus {
    u_long drives0[2];
    u_long drives1[2];
    u_long number_of_drives[2];
    u_long status[2];
    u_long failures[2];
    u_long attempts[2];
    int eot_warning;
    int unit;
};

#ifdef OK_RPC

#include "tapeCommands.h"
#include "tapeStatus.h"

typedef struct TapeCommand TapeCommand;
bool_t xdr_TapeCommand(XDR *, TapeCommand *);

typedef struct TapeStatus TapeStatus;
bool_t xdr_TapeStatus(XDR *, TapeStatus *);

#define TapeControl ((u_long)0x30000300)
#define TapeCtrlVers ((u_long)1)
#define SendCommand ((u_long)1)
#define GetTapeStatus ((u_long)2)

extern struct TapeStatus *sendtapecommand_1(TapeCommand *, CLIENT *);
extern struct TapeStatus *gettapestatus_1(void *, CLIENT *);
extern struct TapeStatus *sendcommand_1_svc(TapeCommand *, struct svc_req *);
extern struct TapeStatus *gettapestatus_1_svc(void *, struct svc_req *);

void startControl(void);

#else
program TapeControl {
    version TapeCtrlVers {
	struct TapeStatus SendTapeCommand(struct TapeCommand) = 1;
	struct TapeStatus GetTapeStatus(void) = 2;
    } = 1;
} = 0x30000300;
#endif /* OK_RPC */      
#endif /* INCtapeControlh */

