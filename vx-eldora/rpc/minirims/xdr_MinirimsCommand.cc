//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          xdr_MinirimsCommand.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#ifndef WRS_SIDE
#ifndef UNIX
#define UNIX
#endif
#endif
#include "MinirimsRpc.hh"

bool_t xdr_MinirimsCommand(register XDR *xdrs, MinirimsCommand *objp)
{
  register long *buf;

  if (!xdr_u_long(xdrs, &objp->cmd))
    return (FALSE);
  return (TRUE);
}
