/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 FixedPipe.cc
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
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "FixedPipe.hh"

int FixedPipe::Read(FAST void *ptr, FAST int timeout)
{
    FAST SEM_ID rs = read_sem;
    FAST SEM_ID ws = write_sem;

    if (semTake(rs,timeout) == ERROR)
      return(0);

    FAST int bytes = Get(ptr);

    semGive(ws);

    if (!Empty())
      semGive(rs);

    return(bytes);
}

int FixedPipe::Write(FAST void *ptr, FAST int timeout)
{
    FAST SEM_ID rs = read_sem;
    FAST SEM_ID ws = write_sem;

    if (semTake(ws,timeout) == ERROR)
      return(0);

    FAST int bytes = Put(ptr);

    semGive(rs);
	  
    if (!Full())
      semGive(ws);

    return(bytes);
}

int FixedPipe::QueryRead(FAST void *ptr)
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

    FAST SEM_ID ws = write_sem;

    FAST int bytes = Get(ptr);

    semGive(ws);

    if (!Empty())
      semGive(rs);

    taskUnlock();

    return(bytes);
}
    
int FixedPipe::QueryWrite(FAST void *ptr)
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
	
    FAST SEM_ID rs = read_sem;
    
    if (Full())
      return(0);

    FAST int bytes = Get(ptr);

    semGive(rs);
	
    if (!Full())
      semGive(ws);

    taskUnlock();
    
    return(bytes);
}
