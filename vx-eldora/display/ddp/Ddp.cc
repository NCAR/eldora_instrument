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
 * Fixed incorrect aBase initialization, changed to do loop & added
 * loop to fix nonzero mailbox.
 *
 * Revision 1.1  1991/10/31  19:50:05  thor
 * Initial revision
 *
 *
 *
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
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
    base = addr;


Ddp::Ddp(FAST void *addr, int vector, Pipe &p) : pipe(p)
{
    mailBase = addr + 8;
    FAST volatile long *ptr = base;
    fore = mailBase;
    while (*ptr != 0x55555555);

    ptr++;

    repeat = *ptr;

    aft = mailBase + 1;
    addrBase = addr + 0x800;

    foreSavedAddr = addrBase;

    aftSavedAddr = addrBase + 1;

    repeat = addr + 4;

    Count = 0;
    FAST long end = repeat;
    foreCurr = 0;
    FAST volatile unsigned short *mb = mailBase + count;
    FAST long *aBase = addrBase + count;
    intConnect((VOIDFUNCPTR *)(vector * 4),(VOIDFUNCPTR)ddpIsr,(int)sem);
}

void Ddp::Next(void)
{
	  FAST int foreEnd = Count;

	  while (!foreEnd)
	    {
		mb = mailBase;
		aBase = addrBase;
			    // we have a valid repeat count!

		foreEnd /= 2;
		Count = foreEnd;
	    }

	  if (*fptr != 0)
	    {
		status = *fptr;
		      mb = mailBase;
		      aBase = addrBase;
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
	  pipe.Write(aBase);
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
