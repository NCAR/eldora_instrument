/*
 *	$Id$
 *
 *	Module:		 FixedPipe.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1990/12/27  09:32:49  thor
 * Changed to use WRS' ansi headers and new binary semaphores.
 *
 * Revision 1.1  90/03/30  09:38:20  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This is the header file for the class FixedPipe, which implements
 * an object that acts as a pipe. Any number of tasks may read/write
 * to the FixedPipe. Because FixedPipe is built using the ringBuffer class,
 * FixedPipes require that all read/writes be done in fixed message size
 * units. 
 *
 */
#ifndef INCFixedPipeh
#define INCFixedPipeh

#include "Pipe.h"

class FixedPipe:public Pipe {

  public:

    FixedPipe(int size_of_msg, int num_of_msgs) : (size_of_msg, num_of_msgs)
      {}

/* DO NOT USE the i variable - it DOES NOTHING!!! */
     int read(void *ptr, int i = 0, int timeout = WAIT_FOREVER);
     int write(void *ptr, int i = 0, int timeout = WAIT_FOREVER);

     int query_read(void *ptr, int i = 0);
     int query_write(void *ptr, int i = 0);
};

#endif INCFixedPipeh

