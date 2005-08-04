/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Pipe.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1993/07/28  17:07:11  thor
 * Upgraded to latest ANSI spec.
 *
 * Revision 1.2  1992/06/25  20:45:38  thor
 * Removed unneeded semTakes, added task(un)Lock.
 *
 * Revision 1.1  1991/10/31  18:59:55  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Pipe.hh"

int Pipe::Read(FAST void *ptr, FAST int item_count, FAST int timeout)
{
    FAST int bytes = 0;
    FAST char *buff = (char *)ptr;
    FAST int inc = addr_inc;
    FAST SEM_ID rs = read_sem;
    FAST SEM_ID ws = write_sem;

    for (FAST int i = 0; i < item_count; i++)
      {
	  if (semTake(rs,timeout) == ERROR)
	    return(bytes);

	  bytes += Get(buff);

	  buff += inc;

	  semGive(ws);

	  if (!Empty())
	    semGive(rs);
      }
    return(bytes);
}

int Pipe::Write(FAST void *ptr, FAST int item_count, FAST int timeout)
{
    FAST int bytes = 0;
    FAST char *buff = (char *)ptr;
    FAST int inc = addr_inc;
    FAST SEM_ID rs = read_sem;
    FAST SEM_ID ws = write_sem;

    for (FAST int i = 0; i < item_count; i++)
      {

	  if (semTake(ws,timeout) == ERROR)
	    return(bytes);

	  bytes += Put(buff);

	  buff += inc;

	  semGive(rs);
	  
	  if (!Full())
	    semGive(ws);
      }
    return(bytes);
}

void Pipe::Flush()
{
    taskLock();
    
    Ring::Flush();

    semGive(write_sem);
    taskUnlock();
}

int Pipe::Free(void)
{
    return(Ring::Free());
}

int Pipe::Msgs(void)
{
    return(max_items);
}

int Pipe::Size(void)
{
    return(Ring::Size());
}

int Pipe::QueryRead(FAST void *ptr, FAST int item_count)
{
    taskLock();

    if (Empty())
      {
	  taskUnlock();
	  return(0);
      }

    FAST SEM_ID rs = read_sem;

    if (semTake(rs,NO_WAIT) == ERROR)
      {
	  taskUnlock();
	  return(0);
      }

    FAST int bytes = 0;
    FAST char *buff = (char *)ptr;
    FAST int inc = addr_inc;
    FAST SEM_ID ws = write_sem;

    for (FAST int i = 0; i < item_count; i++)
      {
	  if (Empty())
	    break;

	  bytes += Get(buff);

	  buff += inc;

	  semGive(ws);
      }

    if (!Empty())
      semGive(rs);

    taskUnlock();

    return(bytes);
}
    
int Pipe::QueryWrite(FAST void *ptr, FAST int item_count)
{
    taskLock();

    if (Full())
      {
	  taskUnlock();
	  return(0);
      }

    FAST SEM_ID ws = write_sem;

    if (semTake(ws,NO_WAIT) == ERROR)
	{
	    taskUnlock();
	    return(0);
	}
	
    FAST int bytes = 0;
    FAST char *buff = (char *)ptr;
    FAST int inc = addr_inc;
    FAST SEM_ID rs = read_sem;
    
    for (FAST int i = 0; i < item_count; i++)
      {
	  if (Full())
	    return(0);

	  bytes += Get(buff);

	  buff += inc;

	  semGive(rs);
      }
	
    if (!Full())
      semGive(ws);

    taskUnlock();
    
    return(bytes);
}
