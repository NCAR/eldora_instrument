/*
 *	$Id$
 *
 *	Module:		 __builtin_delete
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 17 Jan 1990
 *
 * revision history
 * ----------------
 * $Log$
 * description:
 *       This routine frees memory allocated by a call to __builtin_new.
 */
#include "vxWorks.h"

#include "builtin.h"

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

void __builtin_delete(void *ptr)
{
    free(ptr);
}
