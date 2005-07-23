//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Mbp.hh
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
#ifndef _MBP_HH
#define _MBP_HH

#include "Pipe.hh"

class Mbp: public Pipe {
public:
  Mbp(void *baseAddr);

  void next();

private:
  void isr() { semGive(sem); }

  int *mb_addr;
  SEM_ID sem;

  friend void glueIsr(Mbp *obj);
};

#endif // _MBP_HH
