/*
 *	$Id$
 *
 *	Module:		 tapeStatus.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/28  19:49:37  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCtapeStatush
#define INCtapeStatush

#include "GblStatus.h"

#define INITIALIZING 0x1
#define PING_PONG_ON 0x2
#define RECORDING   0x4
#define REWINDING   0x8
#define EJECTING    0x10
#define PARALLEL_ON 0x20
#define CLEAN_1     0x40
#define CLEAN_2     0x80
#define MCPL_PROBLEMS 0x100
#define RAY_COUNT_PROBLEMS 0x200
#define MEDIA_ERROR 0x400
#define HARDWARE_ERROR 0x800

#endif /* INCtapeStatush */

