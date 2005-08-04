/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Asynch.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        This class provides a simple means of allowing reporting and
 * waiting on asynchronous events. The state can be used to hold
 * whatever information the caller deems useful. Error holds the
 * success of the call. 
 *
 */
#ifndef INCAsynchhh
#define INCAsynchhh

extern "C" {
#include "vxWorks.h"
#include "semLib.h"
};

static const int ASYNCH_TIMEOUT = -1;

class Asynch {
  private:
    SEM_ID sem;
    
    int nonblocking;
    int state;
    int error;
    int timeOut;

  public:
    Asynch(FAST int nb = 0)
      {
	  nonblocking = nb;

	  if (nb)
	    sem = NULL;
	  else
	    sem = semBCreate(SEM_EMPTY,SEM_Q_PRIORITY);

	  state = 0;

	  error = 0;
      }

    void Signal(void)
      {
	  if (nonblocking)
	    return;

	  semGive(sem);
      }

    STATUS Wait(FAST int tmo)
      {
	  if (nonblocking)
	    return(OK);

	  return(semTake(sem, tmo));
      }

    int State(void)
      {
	  return(state); 
      }

    void State(int s)
      {
	  state = s;
      }

    int Error(void)
      {
	  return(error);
      }

    void Error(int e)
      {
	  error = e;
      }

    int Timeout(void)
      {
	  return(timeOut);
      }

    void Timeout(int tmo)
      {
	  timeOut = tmo;
      }

    ~Asynch(void)
      {
	  semDelete(sem);
      }
};

#endif /* INCAsynchhh */

