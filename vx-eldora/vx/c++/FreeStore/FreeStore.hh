//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          FreeStore.hh
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
//        The FreeStore class allows for easy interfacing to the
//        VxWorks memPart* routines. Currently there are 5 methods of
//        construction - defaulting to the system partion, using a
//        user specified partion, a copy constructor, using an address
//        and length in bytes and using an address as a base. The last
//        is the most complicated - the user specifies a base address,
//        the size of memory accesses (1, 2 or 4 bytes), whether the
//        memory should be read or written, how many memory units to
//        increase the address tested each time and a location to either
//        store read data or get written data from. If the data
//        variable is omitted or NULL, the constructor will use a
//        local variable. The constructor then dynamically sizes the
//        partition by walking through memory until it hits a
//        nonexistent address.
//
//        All the methods map onto the set of functions that are
//        provided to manipulate partitions, except for valloc (at
//        least until I figure out how to do it). 
//
#ifndef _FREESTORE_HH
#define _FREESTORE_HH

#pragma interface

#include <memLib.h>
#include <vxLib.h>

class FreeStore {
  public:
    enum sizing_mode { read = VX_READ, write = VX_WRITE, fixed = -1 };

    // Constructors:
    FreeStore() { partId = memSysPartId; }
    FreeStore(PART_ID id) { partId = id; }
    FreeStore(FreeStore &f) { partId = f.partId; }
    FreeStore(void *address, int length = 4, int mode = FreeStore::fixed,
              void *data = NULL, size_t size = 1024);

    // Methods that allocate memory:
    void *malloc(size_t size) { return(memPartAlloc(partId,(unsigned)size)); }
    void *realloc(void *ptr, size_t size)
      { return(memPartRealloc(partId,(char *)ptr,(unsigned)size)); }
    void *calloc(size_t elemNum, size_t elemSize);
    void *memalign(unsigned alignment, unsigned size)
      { return(memPartAlignedAlloc(partId,size,alignment)); }
    //void *valloc(size_t size);

    // Methods to free memory:
    void free(void *ptr) { memPartFree(partId,(char *)ptr); }
    STATUS cfree(void *ptr) { memPartFree(partId,(char *)ptr); }

    // Methods to size size of free store:
    STATUS add(void *ptr, size_t size)
      { return(memPartAddToPool(partId,(char *)ptr,(unsigned)size)); }

    // Set debug options:
    STATUS setOptions(unsigned int options)
      { return(memPartOptionsSet(partId,options)); }

    // Get size of largest available block.
    int maxBlock() { return(memPartFindMax(partId)); }

    // Show current state of partition:
    STATUS show(int type) { return(memPartShow(partId,type)); }
    
  private:
    PART_ID partId;
};

#endif // _FREESTORE_HH
