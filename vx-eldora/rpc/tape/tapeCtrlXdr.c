/*
 *	$Id$
 *
 *	Module:		 tapeCtrlXdr.c
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/28  19:48:46  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define UNIX
#include "tapeControl.h"

bool_t xdr_TapeCommand(FAST XDR *xdrs, FAST TapeCommand *objp)
{
    if (!xdr_u_long(xdrs,&objp->cmd))
      {
	  return(FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->count))
      {
	  return(FALSE);
      }
    if (!xdr_int(xdrs,&objp->unit))
      {
	  return(FALSE);
      }
    if (!xdr_int(xdrs,&objp->parameter))
      {
	  return(FALSE);
      }
    if (!xdr_float(xdrs,&objp->top))
      {
	  return(FALSE);
      }
    if (!xdr_float(xdrs,&objp->bottom))
      {
	  return(FALSE);
      }
    if (!xdr_float(xdrs,&objp->start))
      {
	  return(FALSE);
      }
    if (!xdr_float(xdrs,&objp->stop))
      {
	  return(FALSE);
      }
    if (!xdr_float(xdrs,&objp->threshold))
      {
	  return(FALSE);
      }
    return (TRUE);
}

bool_t xdr_TapeError(FAST XDR *xdrs, FAST TapeError *objp)
{
    if (!xdr_int(xdrs,&objp->error))
      {
	  return(FALSE);
      }
    return(TRUE);
}

bool_t xdr_ScsiError(FAST XDR *xdrs, FAST ScsiError *objp)
{
    if (!xdr_int(xdrs,&objp->error))
      {
	  return(FALSE);
      }
    return(TRUE);
}

bool_t xdr_TapeStatus(FAST XDR *xdrs, FAST TapeStatus *objp)
{
    if (!xdr_u_long(xdrs,&objp->status))
      {
	  return(FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->count))
      {
	  return(FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->failures))
      {
	  return(FALSE);
      }
    if (!xdr_u_long(xdrs,&objp->attempts))
      {
	  return(FALSE);
      }
    if (!xdr_int(xdrs,&objp->eot_warning))
      {
	  return(FALSE);
      }
    if (!xdr_int(xdrs, &objp->unit))
      {
	  return(FALSE);
      }
    if (!xdr_TapeError(xdrs,&objp->terr))
      {
	  return(FALSE);
      }
    if (!xdr_ScsiError(xdrs,&objp->serr))
      {
	  return(FALSE);
      }
    return(TRUE);
}
