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
    if (!xdr_u_long(xdrs,&objp->count))
      {
	  return(FALSE);
      }
    if (!xdr_u_short(xdrs,&objp->year))
      {
	  return(FALSE);
      }
    if (!xdr_u_short(xdrs,&objp->month))
      {
	  return(FALSE);
      }
    if (!xdr_u_short(xdrs,&objp->day))
      {
	  return(FALSE);
      }
    if (!xdr_u_short(xdrs,&objp->hour))
      {
	  return(FALSE);
      }
    if (!xdr_u_short(xdrs,&objp->minute))
      {
	  return(FALSE);
      }
    if (!xdr_u_short(xdrs,&objp->second))
      {
	  return(FALSE);
      }
    return(TRUE);
}

bool_t xdr_HskpStatus(FAST XDR *xdrs, FAST HskpStatus *objp)
{
    if (!xdr_u_long(xdrs,&objp->count))
      {
	  return(FALSE);
      }
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
    if (!xdr_u_char(xdrs,&objp->ecb))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->slave))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->onTemp))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->offTemp))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->foreDDS))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->aftDDS))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->minirims))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->minirimsTemp))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->clock))
      {
	  return(FALSE);
      }
    if (!xdr_u_char(xdrs,&objp->insitu))
      {
	  return(FALSE);
      }
    return(TRUE);
}
