/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorSetClnt.cc
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

#define OK_RPC
#define UNIX
#define CLIENT_SIDE

#include "ColorRpc.hh"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };


void *colorsetproc_1(register struct ColorCmd *argp, register CLIENT *clnt)
{
        static char res;

        bzero((char *)&res,sizeof(res));
        if (clnt_call(clnt,ColorSetProc,xdr_ColorCmd,(caddr_t)argp,xdr_void,
		      (caddr_t)&res,TIMEOUT) != RPC_SUCCESS)
	  {
	      return (NULL);
	  }
        return ((void *)&res);
}
