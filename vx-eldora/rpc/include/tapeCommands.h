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

#define PP_ON     0x10
#define PP_OFF    0x20
#define REC_ON    0x40
#define REC_OFF   0x80
#define REWIND    0x100
#define EJECT     0x200
#define SET_UNIT  0x400
#define PIE_SLICE 0x800
#define CLIP      0x1000
#define THRESHOLD 0x2000

#endif /* INCtapeCommandsh */

