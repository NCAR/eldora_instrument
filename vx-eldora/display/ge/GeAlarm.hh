/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeAlarm.hh
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
 *        
 */
#ifndef INCGeAlarm
#define INCGeAlarm

class GeAlarm;

#include "GeGlobal.hh"

extern "C" {
#include "semLib.h"
};

class GeAlarm {
  private:
    DispStatus status;

    CLIENT *client;

    int socket;

    int system;

    SEM_ID waitSem;

  public:
    GeAlarm(char *server, int system);

    void Set(int error);

    void Send(void);
    
    void Wait(void);
};



#endif // INCGeAlarm

