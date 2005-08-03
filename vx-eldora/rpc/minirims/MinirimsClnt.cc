//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          MinirimsClnt.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1996/07/18  20:06:28  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#ifndef UNIX
#define UNIX
#endif
#include "MinirimsRpc.hh"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *sendminirimscommand_1(struct MinirimsCommand *argp, CLIENT *clnt)
{
  static int clnt_res;

  memset((char *)&clnt_res, 0, sizeof (clnt_res));
  if (clnt_call(clnt, sendMinirimsCommand,
                (xdrproc_t) xdr_MinirimsCommand, (caddr_t) argp,
                (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
                TIMEOUT) != RPC_SUCCESS)
    {
      return (NULL);
    }
  return (&clnt_res);
}

struct MinirimsStatus *statusminirims_1(FAST void *argp, FAST CLIENT *clnt)
{
  static struct MinirimsStatus clnt_res;

  memset((char *)&clnt_res,0,sizeof(clnt_res));
  if (clnt_call(clnt,statusMinirims,
                (xdrproc_t) xdr_void,(caddr_t) argp,
                (xdrproc_t) xdr_MinirimsStatus,(caddr_t)&clnt_res,
                TIMEOUT) != RPC_SUCCESS)
    {
      return (NULL);
    }
  return(&clnt_res);
}
