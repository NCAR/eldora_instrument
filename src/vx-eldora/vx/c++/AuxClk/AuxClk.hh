//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 AuxClk.hh
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
//        These two classes provide a simple interface to the
//        auxillary clock interrupt. The base class AuxClk simply is a
//        wrapper for the sysAuxClk* routines. AuxDog provides a
//        watchdog class with better time resolution then that
//        provided by WRS' wd* family of routines. One is limited only
//        by the resolution of the auxillary clock (ex. - .0002 sec on
//        the TP41).
//
//        AuxDog can be created with a user sprecified function and
//        data to be passed to that function. This function will be
//        called if AuxDog times out. The length of the time out is
//        set via setTmo(), which takes a double specifing the time
//        out in seconds. start() will start the AuxDog unless it is
//        already running, in which case start() returns ERROR.
//        cancel() returns ERROR if the AuxDog has timed out,
//        otherwise it stops the AuxDog. Restart will first cancel and
//        then start the AuxDog. 
//
#ifndef INCAuxClk_hh
#define INCAuxClk_hh

#include <vxWorks.h>
#include <sysLib.h>

extern "C" {
#include "AuxClk.h"
};

class AuxClk {
  public:
    AuxClk(int r = 1000);
    AuxClk(FUNCPTR action, int arg, int r = 1000);

    int rate() { return(intRate); }

    STATUS rate(int r) { intRate = r; return(sysAuxClkRateSet(r)); }

    void enable() { sysAuxClkEnable(); }

    void disable() { sysAuxClkDisable(); }

    STATUS connect(FUNCPTR action, int arg)
      { return(sysAuxClkConnect(action,arg)); }

    ~AuxClk() { disable(); }
    
  protected:
    int intRate;
};

class AuxDog : public AuxClk {
  public:
    AuxDog(FUNCPTR action = NULL, int arg = 0, int r = 1000);

    int setTmo(double tmo);
    
    int start();

    int cancel();

    void restart() { cancel(); start(); }
    
    ~AuxDog();

  private:
    int trigger;
    int count;
    int running;

    int userArg;
    FUNCPTR userAction;

    void wrapper();
};

#endif // INCAuxClk_hh
