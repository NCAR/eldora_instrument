/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 xdr_Hskp.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/09/24  14:55:58  thor
 * Fixed to use new structure.
 *
 * Revision 1.2  1992/07/20  20:56:10  thor
 * Changed shorts to chars.
 *
 * Revision 1.1  1992/07/13  20:18:58  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "Hskp.hh"

bool_t xdr_HskpCommand(FAST XDR *xdrs, FAST HskpCommand *objp)
{
    if (!xdr_u_long(xdrs,&objp->cmd))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->year))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->month))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->day))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->hour))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->minute))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->second))
      {
	  return(FALSE);
      }
    return(TRUE);
}

bool_t xdr_HskpStatus(FAST XDR *xdrs, FAST HskpStatus *objp)
{
    if (!xdr_u_char(xdrs,&objp->fore))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->aft))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->motorCtrl))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->transPower))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->testPower))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->iru))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->gps))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->ecbgen))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->slvdead))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->onBoverT))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->offBoverT))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->forDDSool))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->aftDDSool))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->minirims))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->minirimsTemp))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->clock))
      {
	  return(FALSE);
      }
    if(!xdr_u_char(xdrs,&objp->insitu))
      {
	  return(FALSE);
      }
    return(TRUE);
}
