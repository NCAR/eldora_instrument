/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 xdr_Radar.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/06/26  18:55:31  thor
 * Added DC offset points variable.
 *
 * Revision 1.1  1992/06/25  17:38:58  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "RadarRpc.hh"

bool_t xdr_RadarCommand(XDR *xdrs, FAST RadarCommand *objp)
{
    if (!xdr_u_long(xdrs,&objp->cmd))
      {
	  return (FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->count))
      {
	  return (FALSE);
      }
    if (!xdr_int(xdrs,&objp->dc_points))
      {
	  return (FALSE);
      }
    return (TRUE);
}

bool_t xdr_RadarStatus(XDR *xdrs, FAST RadarStatus *objp)
{
    if (!xdr_u_long(xdrs,&objp->count))
      {
	  return (FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->rp7))
      {
	  return (FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->mcpl))
      {
	  return (FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->status))
      {
	  return (FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->mailbox))
      {
	  return (FALSE);
      }
    return (TRUE);
}
