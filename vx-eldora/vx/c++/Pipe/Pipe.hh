/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Pipe.hh
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
#ifndef INCPipehh
#define INCPipehh

#include "Ring.hh"

extern "C" {
#include "semLib.h"
#include "taskLib.h"
};

class Pipe : private Ring {

  protected:
    SEM_ID read_sem;
    SEM_ID write_sem;

  public:

    Pipe(int size_of_msg, int num_of_msgs) : Ring(size_of_msg, num_of_msgs)
      {
	  read_sem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
	  write_sem = semBCreate(SEM_Q_PRIORITY,SEM_FULL); 
      }

    int Read(void *ptr, int item_count = 1, int timeout = WAIT_FOREVER);

    int Write(void *ptr, int item_count = 1, int timeout = WAIT_FOREVER);

    int QueryRead(void *ptr, int item_count = 1);

    int QueryWrite(void *ptr, int item_count = 1);

    void Flush();

    int Free();

    int Msgs();

    int Size();

    ~Pipe() 
      {
	  semFlush(write_sem);
	  semFlush(read_sem);
	  semDelete(read_sem);
	  semDelete(write_sem);
      }
};

      

#endif // INCPipehh

