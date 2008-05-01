//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          hskp.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.6  2005/08/03 20:23:02  granger
// updates copied from the versions in the rpc/include directory; some of
// these are commits of changes from /net/eldora/eldora/rpc/hskp which were
// never committed to rcs; and some files are being added to revision control
// for the first time
//
// Revision 1.5  1994/09/13  16:24:18  thor
// Fixed losing clnt_destroy().
//
// Revision 1.4  1994/09/07  15:36:05  thor
// Added missing clnt_destroy().
//
// Revision 1.3  1994/09/06  19:36:21  thor
// Changed name to sendhskpcommand_1.
//
// Revision 1.2  1994/09/06  16:31:55  thor
// Added reconnection method.
//
// Revision 1.1  1994/09/01  18:05:56  thor
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

#include <iostream.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "HskpControl.h"

class messanger {
  public:
    messanger() { client = NULL; }

    void getCmd();

  private:
    int connect();

    int reconnect();
    
    int sendCmd();

    int getStatus();

    HskpCommand cmd;

    HskpStatus *status;

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
    cerr << strlen(buffer) << endl;
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
            cout << clnt_spcreateerror("housekeeper") << endl;
#ifdef DEBUG_ONLY
            cerr << clnt_spcreateerror("housekeeper") << endl;
#endif
        }
    else if (result == -2 || result == -3)
        {
            cout << clnt_sperror(client,"housekeeper") << endl;
#ifdef DEBUG_ONLY
            cerr << clnt_sperror(client,"housekeeper") << endl;
#endif
        }
    else
        {
            cout << "OK" << endl;
            cout << ((int)status->fore & 0xff) << endl;
            cout << ((int)status->aft & 0xff) << endl;
            cout << ((int)status->motorCtrl & 0xff) << endl;
            cout << ((int)status->transPower & 0xff) << endl;
            cout << ((int)status->testPower & 0xff) << endl;
            cout << ((int)status->iru & 0xff) << endl;
            cout << ((int)status->gps & 0xff) << endl;
            cout << ((int)status->ecbgen & 0xff) << endl;
            cout << ((int)status->slvdead & 0xff) << endl;
            cout << ((int)status->onBoverT & 0xff) << endl;
            cout << ((int)status->offBoverT & 0xff) << endl;
            cout << ((int)status->forDDSool & 0xff) << endl;
            cout << ((int)status->aftDDSool & 0xff) << endl;
            cout << ((int)status->minirims & 0xff) << endl;
            cout << ((int)status->minirimsTemp & 0xff) << endl;
            cout << ((int)status->clock & 0xff) << endl;
            cout << ((int)status->insitu & 0xff) << endl;
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
    
    if ((client = clnt_create(target,HSKPCONTROL,HSKPCTRLVERS,"udp")) == NULL)
      return(-1);
    else
      return(getStatus());
}

int messanger::sendCmd()
{
    cin >> cmd.cmd;

    int tmp;
    
    cin >> tmp;
    cmd.year = (unsigned char)tmp;
    cin >> tmp;
    cmd.month = (unsigned char)tmp;
    cin >> tmp;
    cmd.day = (unsigned char)tmp;
    cin >> tmp;
    cmd.hour = (unsigned char)tmp;
    cin >> tmp;
    cmd.minute = (unsigned char)tmp;
    cin >> tmp;
    cmd.second = (unsigned char)tmp;

    cerr << cmd.cmd << endl;
#ifdef DEBUG_ONLY
    cerr << cmd.cmd << endl;
    cerr << cmd.year << endl;
    cerr << cmd.month << endl;
    cerr << cmd.day << endl;
    cerr << cmd.hour << endl;
    cerr << cmd.minute << endl;
    cerr << cmd.second << endl;
#endif    

    if ((status = sendhskpcommand_1(&cmd,client)) == NULL)
      return(-2);

    return(0);
}

int messanger::getStatus()
{
    if ((status = gethskpstatus_1(NULL,client)) == NULL)
      return(-3);
    return (0);
}
    
main(int argc, char **argv)
{
    messanger msgr;

    while (1)
      msgr.getCmd();
}
