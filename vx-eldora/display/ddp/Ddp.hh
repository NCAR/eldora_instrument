/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Ddp.hh
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
#ifndef INCDdphh
#define INCDdphh

#include "Pipe.hh"

extern "C" {
#include "string.h"
#include "intLib.h"
};

class Ddp {
  private:
    Pipe &pipe;

    long *base;
    unsigned short *mailBase;
    long *addrBase;

    long repeat;
    long current;

    SEM_ID sem;

    Task **drawingTask;

  public:
    Ddp(void *addr, int vector, Pipe &p);

    void PostAlarm(void);

    void Next(void);
};

#endif // INCDdphh

