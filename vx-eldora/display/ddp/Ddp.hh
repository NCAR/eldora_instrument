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
 * Revision 1.4  1992/06/29  14:11:48  thor
 * Add Clear method.
 *
 * Revision 1.3  1992/02/27  19:37:21  thor
 * Added missing Task.hh include.
 *
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
    int mailboxCount;
    int radar;

    SEM_ID sem;

    Task **drawingTask;

  public:
    Ddp(void *addr, int vector, Pipe &p);

    void PostAlarm(void);

    void Next(void);
    
    void Clear(void) { taskLock(); sem->semCount = 0; taskUnlock(); }

    void Fore(void) { radar = 0; }
    
    void Aft(void) { radar++; }
};

#endif // INCDdphh

