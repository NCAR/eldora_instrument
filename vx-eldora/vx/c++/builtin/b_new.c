/*
 *	$Id$
 *
 *	Module:		 ___builtin_new
 *	Original Author: Richard E.K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 17 Jan. 1990
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *      This C file contains the needed stdlib function ___builtin_new
 * for use with g++. This function creates a new object if no space has
 * been allocated for it already. If the routine fails, it calls
 * whatever is pointed to by __new_handler. The default value for
 * this is the routine __std_new_handler, included in this file.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "memLib.h"

#define BLTIN_LOCAL
#include "builtin.h"


void *__builtin_new(int size)	/* Note that we use only two _'s, the */
				/* compiler adds the third. */
{
    void *ptr;

    if ((ptr = (void *)malloc(size)) == (void *)NULL) /* If no more room */
      {
	  (*__new_handler)();
      }	  

    return(ptr);
}

void __std_new_handler()	/* Do nothing */
{
    return;
}
