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
 *
 *
 * description:
 *        
 */
#ifndef INCHskpCmdh
#define INCHskpCmdh

struct HskpCommand {
    unsigned long cmd;
    unsigned long count;
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
};

#ifdef OK_RPC
typedef struct HskpCommand HskpCommand;

#include "GblCommands.h"

#define ALIGN       0x10
#define GPS_START   0x20
#define IRIGB_SYNC  0x30
#define SET_TIME    0x40
#define START_CLOCK 0x80

#endif /* OK_RPC */

#endif /* INCHskpCmdh */

