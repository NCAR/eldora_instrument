//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          FreeStore.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1994/11/19 00:04:31  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "FreeStore.hh"
#include <string.h>

FreeStore::FreeStore(void *address, int length, int mode, void *data,
                     FAST size_t size)
{
    if (mode == FreeStore::fixed)
      partId = memPartCreate((char *)address,(unsigned)length);
    else
      {
          int srcSink;          // Default is user hasn't given us one.

          FAST char *tst = (char *)address;

          FAST void *ss;

          if (data == NULL)
            ss = (void *)&srcSink;
          else
            ss = data;

          FAST size_t bytes = 0; // How many bytes available.
          
          while (vxMemProbe(tst,mode,length,(char *)ss) == OK)
            {
                tst += size;
                bytes += size;
            }

          partId = memPartCreate((char *)address,(unsigned)bytes);
      }
}

void *FreeStore::calloc(FAST size_t elemNum,FAST size_t elemSize)
{
    FAST size_t s = elemNum * elemSize;
    
    FAST void *p = malloc(s);
    
    if (p != NULL)              // If we got the memory desired, zero it.
      memset(p,0,s);
    
    return(p);
}
