/*
 *	$Id$
 *
 *	Module:		 Pipe.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1990/12/27  09:33:25  thor
 * Changed to use WRS' ansi headers and new binary semaphores.
 *
 * Revision 1.1  90/03/30  09:37:17  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        This is the header file for the class Pipe, which implements
 * an object that acts as a pipe. Any number of tasks may read/write
 * to the Pipe. Because Pipe is built using the RingBuffer class,
 * Pipes require that all read/writes be done in fixed message size
 * units. However, it is possible to read/write variable size buffers,
 * since one can specify a item_count value that indicates how many
 * messages are to be handled per call.
 *
 */
#ifndef INCPipeh
#define INCPipeh

#include "RingBuffer.h"

extern "C" {
#include "semLib.h"
};

class Pipe:public RingBuffer {

  protected:
    SEM_ID read_sem;
    SEM_ID write_sem;

  public:

    Pipe(int size_of_msg, int num_of_msgs) : (size_of_msg, num_of_msgs)
      {
	  read_sem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
	  write_sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL); 
      }

    virtual int read(void *ptr, int item_count = 1,
		     int timeout = WAIT_FOREVER);
    virtual int write(void *ptr, int item_count = 1,
		      int timeout = WAIT_FOREVER);

    virtual int query_read(void *ptr, int item_count = 1);
    virtual int query_write(void *ptr, int item_count = 1);

    void flush_pipe();

    int num_of_msgs();
    int num_of_bytes();

    ~Pipe() 
      {
	  semFlush(write_sem);
	  semFlush(read_sem);
	  semDelete(read_sem);
	  semDelete(write_sem);
      }
};

      

#endif INCPipeh

