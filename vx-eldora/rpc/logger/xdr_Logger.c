/*
 *	$Id$
 *
 *	Module:		 xdr_Logger.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.1  1993/09/10  16:42:56  thor
 * New improved version!
 *
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1992/01/16  19:42:37  thor
 * Changed char array to size 81.
 *
 * Revision 1.1  1992/01/14  19:18:19  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "logger.h"

bool_t xdr_LOGSOURCE(FAST XDR *xdrs, FAST LOGSOURCE *objp)
{
    if (!xdr_enum(xdrs, (enum_t *)objp))
      {
	  return (FALSE);
      }

    return(TRUE);
}

bool_t xdr_LOGMSG(FAST XDR *xdrs, FAST LOGMSG *objp)
{
    if (!xdr_LOGSOURCE(xdrs,&objp->src)) 
      {
	  return(FALSE);
      }
    if (!xdr_vector(xdrs,(char *)objp->message,81,sizeof(char),xdr_char))
      {
	  return(FALSE);
      }
    if (!xdr_vector(xdrs, (char *)objp->items, 10, sizeof(int), xdr_int))
      {
	  return (FALSE);
      }
    return(TRUE);
}
