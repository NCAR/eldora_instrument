/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 FixedPipe.hh
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
#ifndef INCFixedPipehh
#define INCFixedPipehh


#endif // INC

#ifndef INCFixedPipeh
#define INCFixedPipeh

#include "Pipe.hh"

class FixedPipe : public Pipe {

  public:

    FixedPipe(int size_of_msg, int num_of_msgs) : 
    Pipe(size_of_msg, num_of_msgs)
      {}

     int Read(void *ptr, int timeout = WAIT_FOREVER);
     int Write(void *ptr, int timeout = WAIT_FOREVER);

     int QueryRead(void *ptr);
     int QueryWrite(void *ptr);
};

#endif // INCFixedPipehh
