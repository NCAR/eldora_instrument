/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Ring.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/07/28  16:54:13  thor
 * Upgraded to VxWorks 5.1.
 *
 * Revision 1.1  1991/10/31  15:10:58  thor
 * Initial revision
 *
 *
 *
 * description:
 *        See Ring.hh
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Ring.hh"

Ring::Ring(FAST int size_of_item, FAST int num_of_items = 10)
{
    max_items = num_of_items;

    if ((ring = (void *)calloc(num_of_items,size_of_item)) == (void *)NULL)
      {
	  fprintf(stderr,"Allocation of memory for Ring failed.\n");
	  exit(1);
      }

    gauge = 0;

    max_addr = (char *)ring + ((num_of_items - 1) * size_of_item);
    in_addr = (char *)ring;
    out_addr = (char *)ring;
    
    addr_inc = size_of_item;
    
}

int Ring::Get(FAST void *ptr)
{
    FAST int g = gauge;

    if (g == 0)			// No data to return.
      return(0);

    FAST int num_items = max_items;
    FAST char *max = max_addr;
    FAST int inc = addr_inc;
    FAST  char *to = (char *)ptr;
    FAST char *from = out_addr;

    memmove(to,from,inc);

    gauge = --g;		// One less open slot.

    if ((from += inc) > max)	// See if address needs to roll over.
      from = (char *)ring;

    out_addr = from;		// Update out pointer.

    return(inc);
}

int Ring::Put(FAST void *ptr)
{
    FAST int g = gauge;
    FAST int num_items = max_items;

    if (g == num_items)		// Ring is full.
      return(0);

    FAST char *max = max_addr;
    FAST int inc = addr_inc;
    FAST  char *from = (char *)ptr;
    FAST char *to = (char *)in_addr;

    memmove(to,from,inc);

    gauge = ++g;		// One less free slot.
 
    if ((to += inc) > max)	// See if address needs to roll over.
      to = (char *)ring;

    in_addr = to;		// Update input pointer.

    return(inc);
}

