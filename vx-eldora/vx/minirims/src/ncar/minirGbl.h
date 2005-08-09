/*
 *	$Id$
 *
 *	Module:	minirGbl.h	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 * description: These are the global variables associated with the minirims
 *              navigation routines.
 */

#include "navvars.h"
/* pointers to the data structures */

#ifndef MINIR_SCOPE
#define MINIR_SCOPE extern
#endif /* MIN_SCOPE */

typedef struct nav_vars NAV_VARS;
MINIR_SCOPE NAV_VARS *nav_ptr;
int cmd;
unsigned short *Command, Cmd;


