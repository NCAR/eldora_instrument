/*
 *	$Id$
 *
 *	Module:		 HskpCmd.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/08/26  16:33:06  thor
 * Fixed totally screwed up defines.
 *
 * Revision 1.2  1992/07/20  20:55:53  thor
 * Changed shorts to chars.
 *
 * Revision 1.1  1992/07/13  20:19:01  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCHskpCmdh
#define INCHskpCmdh

struct HskpCommand {
    unsigned long cmd;
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};

#ifdef OK_RPC
typedef struct HskpCommand HskpCommand;

#include "GblCommands.h"

#define DC_REMOVAL  0x10
#define GPS_START   0x20
#define IRIGB_SYNC  0x40
#define SET_TIME    0x80
#define START_CLOCK 0x100
#define ALIGN       0x200

#endif /* OK_RPC */

#endif /* INCHskpCmdh */

