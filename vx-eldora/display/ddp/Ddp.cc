/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Ddp.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.6  1992/06/29  17:30:06  thor
 * Added code to test for current radar type before doing pipe write.
 *
 * Revision 1.5  1992/06/29  14:54:09  thor
 * Optimized Next loop.
 *
 * Revision 1.4  1991/11/13  14:37:48  thor
 * A little optimizing.
 *
 * Revision 1.3  1991/11/11  15:35:10  thor
 * Removed constructor wait code, added repeat count read to Next.
 *
 * Revision 1.2  1991/11/06  14:43:22  thor
 * Fixed incorrect aBase initialization, changed to do loop & added
 * loop to fix nonzero mailbox.
 *
 * Revision 1.1  1991/10/31  19:50:05  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Ddp.hh"

#ifdef GE
#include "GeGlobal.hh"
#include "GeDraw.hh"
#else
#include "PeGlobal.hh"
#endif // GE

static void ddpIsr(SEM_ID);

Ddp::Ddp(FAST void *addr, int vector, Pipe &p) : pipe(p)
{
    mailBase = addr + 8;

    fore = mailBase;
    aft = mailBase + 1;

    addrBase = addr + 0x800;

    foreSavedAddr = addrBase;

    aftSavedAddr = addrBase + 1;

    repeat = addr + 4;

    Count = 0;
    foreCurr = 0;
    aftCurr = 0;

    sem = semCCreate(SEM_Q_FIFO,0);

    intConnect((VOIDFUNCPTR *)(vector * 4),(VOIDFUNCPTR)ddpIsr,(int)sem);
}

void Ddp::Next(void)
{
    FAST long fcount = foreCurr;
    FAST unsigned short status;
    FAST volatile unsigned short *fptr = fore;
    FAST volatile long *foreAddr = foreSavedAddr;
    FAST SEM_ID semaphore = sem;

    do				// Must fall through at least once!
      {
	  semTake(semaphore,WAIT_FOREVER);

	  FAST int foreEnd = Count;

	  while (!foreEnd)
	    {
		FAST volatile long *rep = repeat;
		foreEnd = *rep; // Wait till here to make certain
			    // we have a valid repeat count!

		foreEnd /= 2;
		Count = foreEnd;
	    }

	  if (*fptr != 0)
	    {
		status = *fptr;
		*fptr = 0;

		if (status != 0xbfff)
		  PostAlarm();
		else if (!radar) // We want fore beams!
		  {

		      DataBeam *db = (DataBeam *)*foreAddr;

		      FAST int tmp = (int)db; // This done to correct for
		      tmp += 0x30200000;      // address difference betweem
		      db = (DataBeam *)tmp;   // VMEbus & onboard memory.
		      
		      FAST char rname = db->data.radar_name[0];

// We perform this test to keep from sending incorrect packets.
		      if (rname == 'F')
			pipe.Write(foreAddr);
		  }

		fcount++;

		if (fcount == foreEnd)
		  {
		      foreAddr = addrBase;
		      fptr = mailBase;
		      fcount = 0;
		  }
		else
		  {
		      fptr += 2;
		      foreAddr += 2;
		  }

		foreSavedAddr = foreAddr;
		fore = fptr;
		foreCurr = fcount;		
	    }
	  else
	    {
		FAST long acount = aftCurr;
		FAST volatile unsigned short *aptr = aft;
		FAST volatile long *aftAddr = aftSavedAddr;
		FAST int aftEnd = Count;
		
		if (*aptr != 0)
		  {
		      status = *aptr;
		      *aptr = 0;
		      
		      if (status != 0xbfff)
			PostAlarm();
		      else if (radar) // We want aft beams!
			{
			    DataBeam *db = (DataBeam *)*aftAddr;
			    
			    FAST int tmp = (int)db; // This done to correct for
			    tmp += 0x30200000;      // address difference betweem
			    db = (DataBeam *)tmp;   // VMEbus & onboard memory.
			    
			    FAST char rname = db->data.radar_name[0];
			    
// We perform this test to keep from sending incorrect packets.
			    if (rname == 'A')
			      pipe.Write(aftAddr);
			}
		      acount++;
		      
		      if (acount == aftEnd)
			{
			    aftAddr = addrBase + 1;
			    aptr = mailBase + 1;
			    
			    acount = 0;
			}
		      else
			{
			    aptr += 2;
			    aftAddr += 2;
			}
		      
		      aftSavedAddr = aftAddr;
		      aft = aptr;
		      aftCurr = acount;		
		  }
	    }
      } while (semaphore->semCount);
}

void Ddp::PostAlarm(void)
{
#ifdef GE
    Alarm->Set(DDP_RCV_ERR);
#endif // GE
}

void Ddp::Clear(void)
{
    taskLock();

    Count = 0;
    foreCurr = 0;
    aftCurr = 0;
    fore = mailBase;
    aft = mailBase + 1;
    foreSavedAddr = addrBase;
    aftSavedAddr = addrBase + 1;
    sem->semCount = 0;

    FAST long *ptr = (long *)mailBase;

    while ((int)ptr < (int)addrBase)
      *ptr++ = 0;

    taskUnlock();
}

static void ddpIsr(FAST SEM_ID sem)
{
    semGive(sem);
}
