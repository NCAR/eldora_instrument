//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          mhen.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1994/09/06  16:08:08  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef UNIX
#define UNIX
#endif
#define OK_RPC
#include "RadarRpc.hh"

class messanger {
  public:
    messanger() { client1 = NULL; client2 = NULL; }

    void getCmd();

  private:
    void connect();

    void reset();

    void getClients();

    void getStatus();

    RadarStatus *stat1;
    RadarStatus *stat2;

    CLIENT *client1;
    CLIENT *client2;

    char target1[50];
    char target2[50];
    
    static char *commands[];
};

char *messanger::commands[] = {
        "Init", "Reset", "Exit", "Status"
    };

void messanger::getCmd()
{
    char buffer[10];

    cin >> buffer;

#ifdef DEBUG_ONLY
    cerr << "Got type " << buffer << endl;
#endif
    
    char **ptr = messanger::commands;

    if (!strcmp(buffer,*ptr++))
      connect();
    else if (!strcmp(buffer,*ptr++))
      reset();
    else if (!strcmp(buffer,*ptr++))
        {
            cout << "OK" << endl;
            exit(0);
        }
    else if (!strcmp(buffer,*ptr))
      getStatus();
}

void messanger::connect()
{
    cin >> target1;
    cin >> target2;
    
    getClients();
}

void messanger::reset()
{
    if (client1 != NULL)
      clnt_destroy(client1);
    if (client2 != NULL)
      clnt_destroy(client2);

    getClients();
}

void messanger::getClients()
{
    if ((client1 = clnt_create(target1,RadarControl,RadarCtrlVers,"udp"))
        == NULL)
      cout << clnt_spcreateerror("recorder") << endl;
    else if ((client2 = clnt_create(target2,RadarControl,RadarCtrlVers,
                                    "udp")) == NULL)
      cout << clnt_spcreateerror("recorder") << endl;
    else
      cout << "OK" << endl;
}
    
void messanger::getStatus()
{
    if ((stat1 = getradarstatus_1(NULL,client1)) == NULL)
      cout << clnt_sperror(client1,"recorder") << endl;
    else if ((stat2 = getradarstatus_1(NULL,client2)) == NULL)
      cout << clnt_sperror(client2,"recorder") << endl;
    else
        {
            cout << "OK" << endl;
            cout << ((int)stat1->rp7 & 0xff) << endl;
            cout << ((int)stat1->mcpl & 0xff) << endl;
            cout << ((int)stat1->status & 0xff) << endl;
            cout << ((int)stat1->mailbox & 0xff) << endl;
            cout << ((int)stat2->rp7 & 0xff) << endl;
            cout << ((int)stat2->mcpl & 0xff) << endl;
            cout << ((int)stat2->status & 0xff) << endl;
            cout << ((int)stat2->mailbox & 0xff) << endl;
        }
}
    
main(int argc, char **argv)
{
    messanger msgr;

    while (1)
      msgr.getCmd();
}
