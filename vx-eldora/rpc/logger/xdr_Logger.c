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

bool_t xdr_LOGFILES(FAST XDR *xdrs, FAST LOGFILES *objp)
{
    if (!xdr_enum(xdrs,(enum_t *)objp))
      {
	  return(FALSE);
      }
    return(TRUE);
}

bool_t xdr_LOG(FAST XDR *xdrs, FAST LOG *objp)
{
    if (!xdr_LOGFILES(xdrs,&objp->file)) 
      {
	  return(FALSE);
      }
    if (!xdr_vector(xdrs,(char *)objp->message,81,sizeof(char),xdr_char))
      {
	  return(FALSE);
      }
    return(TRUE);
}
