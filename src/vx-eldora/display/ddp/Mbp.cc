//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Mbp.cc
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
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "iostream.h"
#include "Mbp.hh"
#include "tp41Lib.h"

static void glueIsr(Mbp *obj);

Mbp::Mbp(void *baseAddr) : Pipe(sizeof(void *),100)
{
  if (connectMailbox((VOIDFUNCPTR)glueIsr,(int)this,1) == ERROR)
    cerr << "Failed to connect to mailbox interrupt!" << endl;

  mb_addr = (int *)((char *)baseAddr + 0x7f000);

  sem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
}

void Mbp::next()
{
  FAST SEM_ID s = sem;
  FAST volatile void **ptr = (void **)mb_addr;

  for (;;)
    {
      semTake(s,WAIT_FOREVER);
      Write(*ptr);
    }
}

void glueIsr(FAST Mbp *obj)
{
  obj->isr();
}
