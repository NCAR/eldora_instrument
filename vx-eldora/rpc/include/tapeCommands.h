/*
 *	$Id$
 *
 *	Module:		 tapeCommands.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  1995/02/14  16:14:09  thor
 * Added LOAD_TAPE command.
 *
 * Revision 1.4  1994/09/02  18:21:48  thor
 * major rewrite.
 *
 * Revision 1.3  1992/05/07  16:32:13  thor
 * Added commands for parallel recording on/off.
 *
 * Revision 1.2  1992/01/14  15:21:24  thor
 * Added code for data reduction.
 *
 * Revision 1.1  1991/08/28  19:49:30  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCtapeCommandsh
#define INCtapeCommandsh

#include "GblCommands.h"

#define LOG       0x10
#define REC_ON    0x40
#define REC_OFF   0x80
#define REWIND    0x100
#define EJECT     0x200
#define LOAD_TAPE 0x400
#define INIT_UNIT 0x800
#define USE_OTHER_UNIT 0x1000

#endif /* INCtapeCommandsh */



