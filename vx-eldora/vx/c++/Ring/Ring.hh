/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Ring.hh
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
 *        This class implements a ring buffer. The constructor
 * specifies the number of elements that the buffer holds and the size
 * of each element. Methods are provided to insert and remove
 * elements, determine the number of elements or bytes free, determine
 * if the buffer is full or empty and to flush the buffer.
 *
 * Note that transfer efficiency increases with buffer size.
 *
 */
#ifndef INCRinghh
#define INCRinghh


#ifndef UNIX			/* This is the VxWorks case. */
extern "C" {
#include "vxWorks.h"
#include "memLib.h"
#include "string.h"
};
#else				/* UNIX case. */
#include <stdio.h>

#ifndef FAST			/* Needed! */
#define FAST register
#endif FAST

#define TRUE 1
#define FALSE 0

#endif //UNIX

class Ring {

  protected:			/* These are needed by all inheritors. */
    void *ring;			/* Array used to hold data. */
    char *in_addr;		/* Addr. to insert data. */
    char *out_addr;		/* Addr. to remove data. */
    char *max_addr;		/* Addr. at which to roll over. */
    int addr_inc;		/* Amount to increment addrs. */

    int max_items;		/* Maximum slots in ring. */
    int gauge;			/* How many slots in use. */

  public:
    Ring(int size_of_item, int num_of_items = 10);

    int Get(void *item);

    int Put(void *item);
    
    int Full()			// All slots used?
      {
	  if (gauge == max_items)
	    return(TRUE);
	  else
	    return(FALSE);
      }

    int Empty()			// Empty?
      {
	  if (!gauge)
	    return(TRUE);
	  else
	    return(FALSE);
      }

    int Free()			// # of slots empty.
      {
	  return(max_items - gauge);
      }

    int Size()			// Size in bytes.
      {
	  return(max_items * addr_inc);
      }

    void Flush()		// Set ring to empty state.
      {
	  in_addr = out_addr = (char *)ring;
	  gauge = 0;
      } 
      
    ~Ring()			// Return our data to system.
      {
	  cfree((char*)ring);
      }
};

#endif // INCRinghh

