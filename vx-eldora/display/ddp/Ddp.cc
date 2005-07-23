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
 * Revision 2.3  1994/09/27  18:37:22  thor
 * Removed checking for correct beam, should be done by comsumer.
 *
 * Revision 2.2  1993/12/03  17:09:31  thor
 * Added Isr stuff.
 *
 * Revision 2.1  1993/08/20  17:21:45  thor
 * Brought up to lastest ANSI spec & commented out alarm stuff.
 *
 * Revision 2.0  1992/11/03  12:53:21  thor
 * *** empty log message ***
 *
 * Revision 1.9  1992/10/21  14:02:25  thor
 * A little tighter.
 *
 * Revision 1.8  1992/10/09  14:56:29  thor
 * Tightened code.
 *
 * Revision 1.7  1992/10/02  20:30:38  thor
 * Many changes to use a more correct method of following mcpl input.
 *
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

Ddp::Ddp(FAST void *addr, int vector, Pipe &p) :
Isr(vector,0,(VOIDFUNCPTR)&Ddp::IsrFunction),pipe(p)
{
  mailBase = (unsigned short *)((char *)addr + Ddp::mailbox);

  fore = mailBase;
  aft = mailBase + 1;

  addrBase = (long *)((char *)addr + Ddp::address);

  foreSavedAddr = addrBase;

  aftSavedAddr = addrBase + 1;

  repeat = (long *)((char *)addr + Ddp::number);

  Count = 0;
  foreCurr = 0;
  aftCurr = 0;

  ddpsem = semCCreate(SEM_Q_FIFO,0);
#ifdef CAN_SKIP
  skip = 0;                     // By default don't skip beams.
#endif
}

void Ddp::Next(void)
{
  FAST unsigned short status;
  FAST SEM_ID semaphore = ddpsem;
#ifdef CAN_SKIP
  FAST unsigned int toggle = 0;
#endif

  do				// Must fall through at least once!
    {
      semTake(semaphore,WAIT_FOREVER);

      FAST int end = Count;

      while (!end)
        {
          FAST volatile long *rep = repeat;
          end = *rep;       // Wait till here to make certain
          // we have a valid repeat count!

          end /= 2;
          Count = end;
        }

      FAST volatile unsigned short *fptr = fore;
      
      if ((status = *fptr) != 0)
        {
          FAST long fcount = foreCurr;
          FAST volatile long *foreAddr = foreSavedAddr;
          
          *fptr = 0;

          if ((!radar) && (status == 0xbfff)) // We want fore beams!
            {
#ifdef CAN_SKIP
              if (!skip || !toggle)
#endif
                pipe.Write((void *)foreAddr);
#ifdef CAN_SKIP
              toggle ^= 1;
#endif
            }
          
          fcount++;

          if (fcount == end)
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

          foreSavedAddr = (long *)foreAddr;
          fore = (unsigned short *)fptr;
          foreCurr = fcount;		
        }
      else
        {
          FAST long acount = aftCurr;
          FAST volatile unsigned short *aptr = aft;
          FAST volatile long *aftAddr = aftSavedAddr;
		
          if ((status = *aptr) != 0)
            {
              *aptr = 0;
		      
              if ((radar) && (status == 0xbfff)) // We want aft beams!
                {
#ifdef CAN_SKIP
                  if (!skip || !toggle)
#endif
                    pipe.Write((void *)aftAddr);
#ifdef CAN_SKIP
                  toggle ^= 1;
#endif
                }

              acount++;
		      
              if (acount == end)
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
		      
              aftSavedAddr = (long *)aftAddr;
              aft = (unsigned short *)aptr;
              aftCurr = acount;		
            }
        }
    } while (semaphore->semCount);
}

void Ddp::Clear(void)
{
  taskLock();
  FAST int level = intLock();

  Count = 0;
  foreCurr = 0;
  aftCurr = 0;
  fore = mailBase;
  aft = mailBase + 1;
  foreSavedAddr = addrBase;
  aftSavedAddr = addrBase + 1;
  ddpsem->semCount = 0;

  memset(mailBase,0,Ddp::maxMbox - Ddp::mailbox);
//     FAST long *ptr = (long *)mailBase;

//     while ((int)ptr < (int)addrBase)
//       *ptr++ = 0;

  intUnlock(level);
  taskUnlock();
}

void Ddp::IsrFunction()
{
  semGive(ddpsem);
}

#ifdef CAN_SKIP
void Ddp::doSkip(FAST Header *hdr)
{
  WAVEFORM *w = hdr->Waveform();

  double ms = (double)w->repeat_seq;

  ms *= (double)w->repeat_seq_dwel;

  if (ms <= 9.0)
    skip = 1;
  else
    skip = 0;
}
#endif
