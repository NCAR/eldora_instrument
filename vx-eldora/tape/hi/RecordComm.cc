//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 RecordComm.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
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

#include "RecordComm.hh"
#include <iostream.h>
#include <string.h>

RecordComm::RecordComm()
{
    client = NULL;
}
RecordComm::RecordComm(char *target)
{
    start(target);
    client = NULL;
}

int RecordComm::start(char *target)
{
    if ((client = clnt_create(target,TapeControl,TapeCtrlVers,"udp")) == NULL)
      {
          cerr << "RecordComm: Unable to connect to " << target << "." << endl;
          return(1);
      }

    memset(&cmd,0,sizeof(cmd));

    return(0);
}

TapeStatus *RecordComm::eject()
{
    if (client == NULL)
      return(NULL);
    
    cmd.cmd = EJECT;

    cmd.count++;

    TapeStatus *status = sendcommand_1(&cmd,client);

    if (status != NULL)
      {
          TapeStatus *ret = new TapeStatus;
          memmove(ret,status,sizeof(TapeStatus));
          return(ret);
      }
    else
      return(status);
}

TapeStatus *RecordComm::rewind()
{
    if (client == NULL)
      return(NULL);

    cmd.cmd = REWIND;

    cmd.count++;

    TapeStatus *status = sendcommand_1(&cmd,client);

    if (status != NULL)
      {
          TapeStatus *ret = new TapeStatus;
          memmove(ret,status,sizeof(TapeStatus));
          return(ret);
      }
    else
      return(status);
}

TapeStatus *RecordComm::log()
{
    if (client == NULL)
      return(NULL);

    cmd.cmd = LOG;

    cmd.count++;

    TapeStatus *status = sendcommand_1(&cmd,client);

    if (status != NULL)
      {
          TapeStatus *ret = new TapeStatus;
          memmove(ret,status,sizeof(TapeStatus));
          return(ret);
      }
    else
      return(status);
}

TapeStatus *RecordComm::recordOn()
{
    if (client == NULL)
      return(NULL);

    cmd.cmd = REC_ON;

    cmd.count++;

    TapeStatus *status = sendcommand_1(&cmd,client);

    if (status != NULL)
      {
          TapeStatus *ret = new TapeStatus;
          memmove(ret,status,sizeof(TapeStatus));
          return(ret);
      }
    else
      return(status);
}

TapeStatus *RecordComm::recordOff()
{
    if (client == NULL)
      return(NULL);

    cmd.cmd = REC_OFF;

    cmd.count++;

    TapeStatus *status = sendcommand_1(&cmd,client);

    if (status != NULL)
      {
          TapeStatus *ret = new TapeStatus;
          memmove(ret,status,sizeof(TapeStatus));
          return(ret);
      }
    else
      return(status);
}

TapeStatus *RecordComm::getStatus()
{
    if (client == NULL)
      return(NULL);

    char junk;
    
    TapeStatus *status = gettapestatus_1(&junk,client);

    if (status != NULL)
      {
          TapeStatus *ret = new TapeStatus;
          memmove(ret,status,sizeof(TapeStatus));
          return(ret);
      }
    else
      return(status);
}
