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
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define UNIX
#define CLIENT_SIDE
#define OK_RPC
#include "tapeControl.h"

static struct timeval TIMEOUT = { 5, 0 };

struct TapeStatus *sendcommand_1(FAST TapeCommand *argp, FAST CLIENT *clnt)
{
    static struct TapeStatus res;
    int stat;

    bzero((char *)&res, sizeof(res));
    if (clnt_call(clnt, SendCommand, xdr_TapeCommand, argp, xdr_TapeStatus,
		  &res, TIMEOUT) != RPC_SUCCESS) 
      {
	  return (NULL);
      }
    return (&res);
}

struct TapeStatus *gettapestatus_1(FAST void *argp, FAST CLIENT *clnt)
{
    static struct TapeStatus res;

    bzero((char *)&res, sizeof(res));
    if (clnt_call(clnt, GetTapeStatus, xdr_void, argp, xdr_TapeStatus, &res,
		  TIMEOUT) != RPC_SUCCESS) 
      {
	  return (NULL);
      }
    return (&res);
}
