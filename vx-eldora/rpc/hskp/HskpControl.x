/*
 * $Id$
 *
 * housekeeping control and status RPC definitions
 *
 * Use "rpcgen -N" to process this file.
 */

struct HskpCommand {
    unsigned long cmd;
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};
typedef struct HskpCommand HskpCommand;

struct HskpStatus {
    unsigned char fore;
    unsigned char aft;
    unsigned char motorCtrl;
    unsigned char transPower;
    unsigned char testPower;
    unsigned char iru;
    unsigned char gps;
    unsigned char ecbgen;
    unsigned char slvdead;
    unsigned char onBoverT;
    unsigned char offBoverT;
    unsigned char forDDSool;
    unsigned char aftDDSool;
    unsigned char minirims;
    unsigned char minirimsTemp;
    unsigned char clock;
    unsigned char insitu;
};

typedef struct HskpStatus HskpStatus;

program HSKPCONTROL {
  version HSKPCTRLVERS {
    HskpStatus SENDHSKPCOMMAND(struct HskpCommand) = 1;
    HskpStatus GETHSKPSTATUS(struct HskpStatus) = 2;
  } = 1;
} = 0x30000500;

/*
 * Extra stuff we need in the header file
 */
#ifdef RPC_HDR
%#include <HskpCommandDefs.h>
%#include <HskpStatusDefs.h>
%#ifndef HSKP_RPC_SCOPE
%#define HSKP_RPC_SCOPE extern
%#endif
%HSKP_RPC_SCOPE HskpStatus *currStatus; /* Our current status. */
#endif /* RPC_HDR */


