/*
 *	$Id$
 *
 *	Module:	minGbl.h	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/10/29  23:03:55  eric
 * Initial revision
 *
 *
 * Initial revision
 *
 *
 * description: These are the global variables associated with the minirims
 *              vme board.
 */


/* pointers to the display field and data structures */
#include "minDisp.h"
#include "semLib.h"

#ifndef MIN_SCOPE
#define MIN_SCOPE extern
#endif /* MIN_SCOPE */
#ifdef OK_RPC
typedef struct mini_Ov1 MINI_OV1;
typedef struct mini_data MINI_D;

MIN_SCOPE MINI_OV1 *ovp;
MIN_SCOPE MINI_D *dp;
MIN_SCOPE SEM_ID main_sem;
MIN_SCOPE volatile int Stop;
MIN_SCOPE struct RIMS_DATA *rims_d;    /* Minirims data */
int cmd;
unsigned short *Command, Cmd;

#endif /* OK_RPC */

