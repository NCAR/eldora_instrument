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
 * Revision 1.6  1994/09/06  16:15:48  thor
 * Updated to TLIRPC, among other changes.
 *
 * Revision 1.5  1993/07/23  14:56:40  thor
 * Added TASS stuff.
 * >> .
 *
 * Revision 1.4  1992/09/23  14:58:47  thor
 * Changed to use new status structure.
 *
 * Revision 1.3  1992/09/23  14:53:37  eric
 * no changes.
 *
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
    if (!xdr_double(xdrs, &objp->dist))
      return (FALSE);
    if (!xdr_int(xdrs,&objp->dc_points))
      {
	  return (FALSE);
      }
#ifdef TASS
    if (!xdr_ts_acq(xdrs, &objp->tass_info))
      {
	  return (FALSE);
      }
#endif /* TASS */
    return (TRUE);
}

bool_t xdr_RadarStatus(XDR *xdrs, FAST RadarStatus *objp)
{
    if (!xdr_char(xdrs,&objp->rp7))
      {
	  return (FALSE);
      }
    if (!xdr_char(xdrs,&objp->mcpl))
      {
	  return (FALSE);
      }
    if (!xdr_short(xdrs,&objp->status))
      {
	  return (FALSE);
      }
    if (!xdr_short(xdrs,&objp->mailbox))
      {
	  return (FALSE);
      }
    return (TRUE);
}
