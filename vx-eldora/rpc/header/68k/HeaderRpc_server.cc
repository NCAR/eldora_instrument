//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 HeaderRpc_server.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "HeaderRpc.h"
#include <fstream.h>

int *headerrpcfunc_1_svc(FAST struct headername *argp,
                         FAST struct svc_req *rqstp)
{
    static int result;

    if (Hdr == NULL)            // Must be the first time through.
      Hdr = new Header;

    char *file = &argp->name[0];
    
    ifstream input(file);

    if ((result = input.good())) // If we open file read it into object.
        {
            input >> *Hdr;

            result = input.good();
        }
    return(&result);
}
