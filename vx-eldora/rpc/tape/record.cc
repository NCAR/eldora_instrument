//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          record.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.4  1994/09/07  15:43:12  thor
// Added missing clnt_destroy().
//
// Revision 1.3  1994/09/06  18:50:58  thor
// Changed name of sendcommand_1 due to name space clashes.
//
// Revision 1.2  1994/09/06  16:35:38  thor
// Added reconnect method.
//
// Revision 1.1  1994/09/02  18:22:04  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define OK_RPC
#define UNIX
extern "C" {
#include "tapeControl.h"
};

class messanger {
  public:
    messanger() { client = NULL; }

    void getCmd();

  private:
    int connect();

    int reconnect();

    int sendCmd();

    int getStatus();

    TapeCommand cmd;

    TapeStatus *status;

    CLIENT *client;

    char target[50];
    
    static char *commands[];
};

char *messanger::commands[] = {
        "Init", "Reset", "Exit", "Change", "Status"
    };

void messanger::getCmd()
{
    char buffer[10];

    cin >> buffer;

#ifdef DEBUG_ONLY
    cerr << "Got type " << buffer << endl;
#endif
    
    int result = 0;

    char **ptr = messanger::commands;

    if (!strcmp(buffer,*ptr++))
      result = connect();
    else if (!strcmp(buffer,*ptr++))
      result = reconnect();
    else if (!strcmp(buffer,*ptr++))
        {
            cout << "OK" << endl;
            exit(0);
        }
    else if (!strcmp(buffer,*ptr++))
      result = sendCmd();
    else if (!strcmp(buffer,*ptr))
      result = getStatus();

    if (result == -1)
        {
            cout << clnt_spcreateerror("recorder") << endl;
#ifdef DEBUG_ONLY
            cerr << clnt_spcreateerror("recorder") << endl;
#endif
        }
    else if (result == -2 || result == -3)
        {
            cout << clnt_sperror(client,"recorder") << endl;
#ifdef DEBUG_ONLY
            cerr << clnt_sperror(client,"recorder") << endl;
#endif
        }
    else
        {
            cout << "OK" << endl;
            cout << status->drives0[0] << endl;
            cout << status->drives0[1] << endl;
            cout << status->drives1[0] << endl;
            cout << status->drives1[1] << endl;
            cout << status->number_of_drives[0] << endl;
            cout << status->number_of_drives[1] << endl;
            cout << status->status[0] << endl;
            cout << status->status[1] << endl;
            cout << status->failures[0] << endl;
            cout << status->failures[1] << endl;
            cout << status->attempts[0] << endl;
            cout << status->attempts[1] << endl;
            cout << status->eot_warning << endl;
            cout << status->unit << endl;
#ifdef DEBUG_ONLY
            cerr << "OK" << endl;
            sleep(5);
#endif
        }
}

int messanger::connect()
{
    cin >> target;
    return(reconnect());
}

int messanger::reconnect()
{
    if (client != NULL)
      clnt_destroy(client);
    
    if ((client = clnt_create(target,TapeControl,TapeCtrlVers,"udp")) == NULL)
      return(-1);
    else
      return(getStatus());
}

int messanger::sendCmd()
{
    cin >> cmd.cmd;
#ifdef DEBUG_ONLY
    cerr << cmd.cmd << endl;
#endif    

    if ((status = sendtapecommand_1(&cmd,client)) == NULL)
      return(-2);

    return(0);
}

int messanger::getStatus()
{
    if ((status = gettapestatus_1(NULL,client)) == NULL)
      return(-3);
    return (0);
}
    
main(int argc, char **argv)
{
    messanger msgr;

    while (1)
      msgr.getCmd();
}
