//
//	$Id$
//
//	Module:		 HeaderRpc_xdr.c
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "HeaderRpc.h"

bool_t xdr_headername(register XDR *xdrs, register headername *objp)
{
	if (!xdr_vector(xdrs,(char *)objp->name,256,
		sizeof(char),(xdrproc_t)xdr_char))
		return(FALSE);
	return(TRUE);
}
