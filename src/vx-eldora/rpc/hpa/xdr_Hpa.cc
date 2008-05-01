/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		xdr_Hpa.cc 
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/02  20:46:22  thor
 * First offical ELDORA release!
 *
 * Revision 2.0  1992/11/02  20:46:22  thor
 * First offical ELDORA release!
 *
 * Revision 1.1  1992/08/12  19:35:38  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "HpaRpc.hh"

bool_t xdr_HPA_CMD(XDR *xdrs, HPA_CMD *objp)
{
    if (!xdr_enum(xdrs,(enum_t *)objp))
      {
	  return(FALSE);
      }
    return(TRUE);
}
