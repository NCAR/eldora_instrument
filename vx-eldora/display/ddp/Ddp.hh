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
 * Revision 2.0  1992/11/03  12:53:21  thor
 * First offical ELDORA release!
 *
 * Revision 1.7  1992/10/09  14:56:29  thor
 * Added comments.
 *
 * Revision 1.6  1992/10/02  20:30:38  thor
 * Many changes to use a more correct method of following mcpl input.
 *
 * Revision 1.5  1992/06/29  17:29:39  thor
 * Added Fore & Aft methods.
 *
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
#include "Isr.hh"

#include "string.h"
#include "intLib.h"

class Ddp : private Isr {
  private:
    SEM_ID ddpsem;		// Semaphore given by ISR.
    
    Pipe &pipe;			// Where the data goes.

    long *repeat;		// Pointer to how many mailboxes.
    unsigned short *mailBase;	// Their base address.
    unsigned short *fore;	// Current fore mailbox address.
    unsigned short *aft;	// Current aft mailbox address.
    long *addrBase;		// Address base.
    long *foreSavedAddr;	// Current fore.
    long *aftSavedAddr;		// Current aft.

    int Count;			// How many mail boxes. Same for fore & aft.
    int foreCurr;		// How many fore mailboxes were checked.
    int aftCurr;		// How many aft mailboxes were checked.
    int radar;			// Use fore or aft beams.

    void IsrFunction();

  public:
    Ddp(void *addr, int vector, Pipe &p);

    void PostAlarm(void);

    void Next(void);
    
    void Clear(void);

    void Fore(void) { radar = 0; }
    
    void Aft(void) { radar++; }
};

#endif // INCDdphh

