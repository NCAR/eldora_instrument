/*
 *	$Id$
 *
 *	Module:	globals	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:21:56  craig
 * Initial revision
 *
 *
 * description: This module should be loaded first before any other
 *              Housekeeping processor module as it declares all of
 *              the global variables 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope
#define define_globals
#include "hskpAll.h"

globals()
{
printf("I am here to declare all the global variables");
printf(" so everyone can use them\n");
}

