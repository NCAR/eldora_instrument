//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          loggerClient.cc
//      Original Author: Richard E. Neitzel
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

#define CLIENT_SIDE
#define LOGGER_SCOPE
#include "logger.h"

#include "string.h"
#include "hostLib.h"
#include "taskLib.h"
#include "semLib.h"

#include "udpClnt.hh"

extern "C" {
  void loggerEvent(FAST char *message, FAST int *ip , int num);
  int loggerInit(FAST int src);
};

static LOGMSG log;		// Outgoing packet.
static udpClnt *clnt;		// Connection to outside.
static SEM_ID guard;		// One at a time & in order.

void loggerEvent(FAST char *message, FAST int *ip , int num)
{
  semTake(guard,WAIT_FOREVER);

  strncpy(&log.message[0],message,80);

  if (num > 10)
    num = 10;
    
  if (ip != NULL && num > 0)
    memcpy(&log.items[0],ip,num*sizeof(int));

  clnt->sendto((void *)&log,sizeof(log));

  semGive(guard);
}

int loggerInit(FAST int src)
{
  log.src = src;

  clnt = new udpClnt("master",loggerPort);

  if ((guard = semBCreate(SEM_Q_FIFO,SEM_FULL)) == NULL)
    return(ERROR);

  semGive(guard);

  return(OK);
}
