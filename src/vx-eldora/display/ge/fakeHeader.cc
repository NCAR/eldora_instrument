//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          fakeHeader.cc
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

#define OK_RPC
#define UNIX
#include "Header.hh"
#include "HeaderRpc.h"
#include <stdlib.h>
#include <unistd.h>

static const char *nettype = "udp";

main(int argc, char **argv)
{
    headername header;

    if (argc < 2)
        {
            cerr << "Sorry, but you must specify a header file." << endl;
            exit(1);
        }

    strcpy(header.name,argv[1]);

    while (1)
        {
            char target[30];

            cout << "Enter the target name [. to quit]: " << flush;
            cin >> target;

            if (*target == '.')
              exit(0);

            CLIENT *client = clnt_create(target,HeaderRpc,HeaderRpcVers,
                                         nettype);

            if (client == NULL)
              cerr << clnt_spcreateerror(argv[0]) << endl;
            else
                {
                    if (headerrpcfunc_1(&header,client) == NULL)
                      cerr << clnt_sperror(client,argv[0]) << endl;
                    else
                      clnt_destroy(client);
                }
        }
}

                            
