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
// Revision 1.1  1994/08/30  15:21:34  thor
// Initial revision
//
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
    static int firstTime = 1;

    if (firstTime)            // Must be the first time through.
        {
            Hdr = new Header();
            firstTime = 0;
        }

    char *file = &argp->name[0];
    
    ifstream input(file);

    if ((result = input.good())) // If we open file read it into object.
        {
            input >> *Hdr;

            result = input.good();
        }
    else
      cout << "Bad open of file " << file << endl;

    return(&result);
}
