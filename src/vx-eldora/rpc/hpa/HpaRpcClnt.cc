/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 HpaRpcClnt.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.1  1993/09/02  13:28:43  thor
 * Made changes to compile under gcc.
 *
 * Revision 2.0  1992/11/02  20:46:22  thor
 * First offical ELDORA release!
 *
 * Revision 1.1  1992/08/12  19:35:43  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define HPA_CLIENT_SIDE

#include "HpaRpc.hh"
#include <string.h>

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

void *hpasendcmd_1(enum HPA_CMD *argp, CLIENT *clnt)
{
    static char res;
    
    memset((char *)&res,0,sizeof(res));
    if (clnt_call(clnt,HpaSendCmd,xdr_HPA_CMD,argp,xdr_void,&res,
		  TIMEOUT) != RPC_SUCCESS)
      {
	  return(NULL);
      }
    return((void *)&res);
}