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
 * Revision 1.3  1994/09/07  14:35:01  thor
 * Renamed sendcommand.
 *
 * Revision 1.2  1994/09/06  16:15:48  thor
 * Updated to TLIRPC, among other changes.
 *
 * Revision 1.1  1992/06/25  17:38:56  thor
 * Initial revision
 *
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

struct RadarStatus *sendradarcommand_1(struct RadarCommand *argp, CLIENT *clnt)
{
    static struct RadarStatus res;
    
    memset((char *)&res,0,sizeof(res));

    if (clnt_call(clnt,SendRadarCommand,(xdrproc_t)xdr_RadarCommand,
                  (caddr_t)argp,(xdrproc_t)xdr_RadarStatus,(caddr_t)&res,
                  TIMEOUT) != RPC_SUCCESS)
      {
	  return(NULL);
      }
    return(&res);
}

struct RadarStatus *getradarstatus_1(void *argp, CLIENT *clnt)
{
    static struct RadarStatus res;
    
    memset((char *)&res,0,sizeof(res));

    if (clnt_call(clnt,GetRadarStatus,(xdrproc_t)xdr_void,(caddr_t)argp,
                  (xdrproc_t)xdr_RadarStatus,(caddr_t)&res,TIMEOUT)
        != RPC_SUCCESS) 
      {
	  return(NULL);
      }
    return(&res);
}
