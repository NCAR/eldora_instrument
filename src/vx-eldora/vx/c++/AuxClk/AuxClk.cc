//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 AuxClk.cc
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

#include "AuxClk.hh"
#include <math.h>
#include <intLib.h>

AuxClk::AuxClk(int r)
{
    rate(r);
}

AuxClk::AuxClk(FUNCPTR action, int arg, int r)
{
    rate(r);
    connect(action,arg);
}

AuxDog::AuxDog(FUNCPTR action, int arg, int r) :
    AuxClk((FUNCPTR)&AuxDog::wrapper,(int)this,r)
{
    userAction = action;
    userArg = arg;
    count = 0;
    running = 0;
}

int AuxDog::setTmo(double tmo)
{
    if (tmo < 0.0)
      return(-1);

    trigger = iround(tmo * (double)intRate);

    return(1);
}

int AuxDog::start()
{
    if (running)
      return(ERROR);
    
    count = 0;
    running = 1;
    enable();
    return(OK);
}

int AuxDog::cancel()
{
    int level = intLock();
    
    if (!running)
      {
          intUnlock(level);
          return(ERROR);
      }

    disable();
    
    running = 0;

    intUnlock(level);
     return(OK);
}

AuxDog::~AuxDog()
{
    cancel();
}

void AuxDog::wrapper()
{
    count++;

    if (count >= trigger)
      {
          disable();
          running = 0;
          if (userAction != NULL)
            (userAction)(userArg);
      }
}

// Now the C interface routines.

AuxDogPtr AuxDogCreate(FUNCPTR func, int arg, int rate)
{
    AuxDog *p = new AuxDog(func,arg,rate);
    
    return((AuxDogPtr)p);
}


int AuxDogSetTmo(AuxDogPtr ptr, double tmo)
{
    AuxDog *p = (AuxDog *)ptr;

    return(p->setTmo(tmo));
}

int AuxDogStart(AuxDogPtr ptr)
{
    AuxDog *p = (AuxDog *)ptr;

    return(p->start());
}

int AuxDogCancel(AuxDogPtr ptr)
{
    AuxDog *p = (AuxDog *)ptr;

    return(p->cancel());
}

void AuxDogRestart(AuxDogPtr ptr)
{
    AuxDog *p = (AuxDog *)ptr;

    p->restart();
}

void AuxDogDestroy(AuxDogPtr ptr)
{
    AuxDog *p = (AuxDog *)ptr;

    delete(p);
}

