/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 HskpClient.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/09/01  18:05:25  thor
 * Updated for use with TLIRPC. Removed unneeded count field.
 *
 * Revision 1.1  1992/07/13  20:18:55  thor
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
#include <memory.h>

static struct timeval TIMEOUT = { 25, 0 };

struct HskpStatus *sendhskpcommand_1(FAST struct HskpCommand *argp,
				 FAST CLIENT *clnt)
{
    static struct HskpStatus res;
    
    memset((char *)&res,0,sizeof(res));
    if (clnt_call(clnt,SendHskpCommand,(xdrproc_t)xdr_HskpCommand,
                  (caddr_t)argp,(xdrproc_t)xdr_HskpStatus,(caddr_t)&res,
                  TIMEOUT) != RPC_SUCCESS)
      {
	  return(NULL);
      }
    return(&res);
}

struct HskpStatus *gethskpstatus_1(FAST void *argp, FAST CLIENT *clnt)
{
    static struct HskpStatus res;
	
    memset((char *)&res,0,sizeof(res));
    if (clnt_call(clnt,GetHskpStatus,(xdrproc_t)xdr_void,(caddr_t)argp,
                  (xdrproc_t)xdr_HskpStatus,(caddr_t)&res,TIMEOUT)
        != RPC_SUCCESS)
        {
            return(NULL);
        }
    return(&res);
}
