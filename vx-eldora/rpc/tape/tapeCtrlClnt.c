/*
 *	$Id$
 *
 *	Module:		 tapeCtrlClnt.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/09/02  18:20:32  thor
 * Major rewrite.
 *
 * Revision 1.1  1991/08/28  19:48:54  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define UNIX
#define CLIENT_SIDE
#define OK_RPC
#include "tapeControl.h"
#include <memory.h>

static struct timeval TIMEOUT = { 5, 0 };

struct TapeStatus *sendtapecommand_1(FAST TapeCommand *argp, FAST CLIENT *clnt)
{
    static struct TapeStatus res;
    int stat;

    (void)memset((char *)&res,0,sizeof(res));
    if (clnt_call(clnt,SendCommand,(xdrproc_t)xdr_TapeCommand,(caddr_t)argp,
                  (xdrproc_t)xdr_TapeStatus,(caddr_t)&res,TIMEOUT)
        != RPC_SUCCESS) 
      {
	  return (NULL);
      }
    return (&res);
}

struct TapeStatus *gettapestatus_1(FAST void *argp, FAST CLIENT *clnt)
{
    static struct TapeStatus res;

    (void)memset((char *)&res,0,sizeof(res));
    if (clnt_call(clnt,GetTapeStatus,(xdrproc_t)xdr_void,(caddr_t)argp,
                  (xdrproc_t)xdr_TapeStatus,(caddr_t)&res,TIMEOUT)
        != RPC_SUCCESS) 
      {
	  return (NULL);
      }
    return (&res);
}
