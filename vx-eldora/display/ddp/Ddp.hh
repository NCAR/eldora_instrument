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
 * Revision 1.2  1991/11/11  15:34:22  thor
 * Removed constructor wait code, added repeat count read to Next.
 *
 * Revision 1.1  1991/10/31  19:50:07  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCDdphh
#define INCDdphh

#include "Pipe.hh"
#include "Task.hh"

extern "C" {
#include "string.h"
#include "intLib.h"
};

class Ddp {
  private:
    Pipe &pipe;

    long *repeat;
    unsigned short *mailBase;
    long *addrBase;

    long current;

    SEM_ID sem;

    Task **drawingTask;

  public:
    Ddp(void *addr, int vector, Pipe &p);

    void PostAlarm(void);

    void Next(void);
};

#endif // INCDdphh

