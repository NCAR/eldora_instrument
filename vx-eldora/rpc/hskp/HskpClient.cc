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
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "Hskp.hh"

static struct timeval TIMEOUT = { 25, 0 };

struct HskpStatus *sendcommand_1(FAST struct HskpCommand *argp,
				 FAST CLIENT *clnt)
{
    static struct HskpStatus res;
    
    bzero((char *)&res,sizeof(res));
    if (clnt_call(clnt,SendCommand,xdr_HskpCommand,argp,xdr_HskpStatus, 
		  &res, TIMEOUT) != RPC_SUCCESS)
      {
	  return(NULL);
      }
    return(&res);
}

struct HskpStatus *gethskpstatus_1(FAST void *argp, FAST CLIENT *clnt)
{
        static struct HskpStatus res;
	
        bzero((char *)&res,sizeof(res));
        if (clnt_call(clnt,GetHskpStatus,xdr_void,argp,xdr_HskpStatus,&res,
		      TIMEOUT) != RPC_SUCCESS)
	  {
	      return(NULL);
	  }
        return(&res);
    }
