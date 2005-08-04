//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          ftst.cc
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
//        Tests 3 cases - default system partion, address & size and
//        dynamically sized.
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdlib.h>
#include <taskLib.h>
#include <iostream.h>
#include "FreeStore.hh"

extern "C" {
    void ftst1();
    void ftst2(void *adr, int l);
    void ftst3(void *adr);
};

void ftst1()
{
    FreeStore sys;

    memShow(1);
    taskDelay(120);
    sys.show(1);                // Should be the same.
    taskDelay(120);
    
    // Now soak up all memory.

    FAST int size = 65536;

    FAST char *p;

    p = (char *)malloc(size);

    memShow(0);
    taskDelay(120);
    sys.show(0);                // Should be the same.
    taskDelay(120);
    
    sys.free(p);

    memShow(0);
    taskDelay(120);
    sys.show(0);                // Should be the same.
}

void ftst2(void *adr, int l)
{
    FreeStore mem(adr,l);

    mem.show(1);

    FAST int size = 65536;
    
    void *p = mem.malloc(size);

    mem.show(1);

    p = mem.realloc(p,size*2);

    mem.show(0);

    mem.free(p);

    mem.show(0);
    
    p = mem.calloc(100,100);

    mem.show(0);

    mem.free(p);
}

void ftst3(void *adr)
{
    FreeStore mem(adr,4,FreeStore::read);

    mem.show(1);

    FAST int size = 65536;
    
    void *p = mem.malloc(size);

    mem.show(1);

    p = mem.realloc(p,size*2);

    mem.show(0);

    mem.free(p);

    mem.show(0);
    
    p = mem.calloc(100,100);

    mem.show(0);

    mem.free(p);

    mem.show(0);
    
    cout << mem.maxBlock() << endl;
}
