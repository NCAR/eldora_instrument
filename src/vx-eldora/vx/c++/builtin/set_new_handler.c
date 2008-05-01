/*
 *	$Id$
 *
 *	Module:		 set_new_handler
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 17 Jan 1990
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/02/11  18:37:53  thor
 * Initial revision
 *
 *
 * description:
 *       This routine allows the user to insert their own handler code
 * to be called when __builtin_new (and hence new) fails.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"

#include "builtin.h"

PF set_new_handler(PF new_hand)
{
    __new_handler = new_hand;
}
