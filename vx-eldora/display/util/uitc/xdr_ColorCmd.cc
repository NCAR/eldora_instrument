/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 xdr_ColorCmd.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/11/20  20:42:10  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC

#ifndef WRS
#define UNIX
#endif

#include "ColorRpc.hh"

bool_t xdr_ColorCmd(FAST XDR *xdrs, FAST ColorCmd *objp)
{
    if (!xdr_int(xdrs,&objp->index))
      {
	  return (FALSE);
      }
    if (!xdr_vector(xdrs,(char *)objp->colors,256,sizeof(long),
		    (xdrproc_t)xdr_long))
      {
	  return (FALSE);
      }
    return (TRUE);
}
