/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RadarCmdClnt.cc
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
#define CLIENT_SIDE
#include "RadarRpc.hh"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

struct RadarStatus *sendcommand_1(struct RadarCommand *argp, CLIENT *clnt)
{
    static struct RadarStatus res;
    
    bzero((char *)&res,sizeof(res));

    if (clnt_call(clnt,SendCommand,xdr_RadarCommand,argp,
		  xdr_RadarStatus,&res,TIMEOUT) != RPC_SUCCESS)
      {
	  return(NULL);
      }
    return(&res);
}

struct RadarStatus *getradarstatus_1(void *argp, CLIENT *clnt)
{
    static struct RadarStatus res;
    
    bzero((char *)&res,sizeof(res));

    if (clnt_call(clnt,GetRadarStatus,xdr_void,argp,xdr_RadarStatus,
		  &res,TIMEOUT) != RPC_SUCCESS) 
      {
	  return(NULL);
      }
    return(&res);
}
