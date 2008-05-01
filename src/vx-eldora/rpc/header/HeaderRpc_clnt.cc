//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          HeaderRpc_clnt.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#ifndef UNIX
#define UNIX
#endif
#ifndef OK_RPC
#define OK_RPC
#endif

#include "HeaderRpc.h"
#include <memory.h>

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *headerrpcfunc_1(struct headername *argp, CLIENT *clnt)
{
    static int clnt_res;

    memset((char *)&clnt_res,0,sizeof(clnt_res));
    if (clnt_call(clnt,HeaderRpcFunc,
                  (xdrproc_t)xdr_headername,(caddr_t)argp,
                  (xdrproc_t)xdr_int,(caddr_t)&clnt_res,
                  TIMEOUT) != RPC_SUCCESS)
      {
          return(NULL);
      }
    return(&clnt_res);
}
