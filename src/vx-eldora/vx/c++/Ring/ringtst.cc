#include "Ring.hh"

extern "C" { 
#include "rngLib.h"
#include "timexLib.h"
void ring(int s, int n);
};

static WrsTest(RING_ID rid, int ni, int nb);
static NcarTest(Ring &, int ni);

static int buffer[1024];

void ring(int s, int n)
{
    RING_ID wrsRing = rngCreate(s * n);
    Ring ncarRing(s,n);

    timexN((FUNCPTR)WrsTest,(int)wrsRing,n,s);

    timexN((FUNCPTR)NcarTest,(int)&ncarRing,n);

    rngDelete(wrsRing);
}

static WrsTest(FAST RING_ID rid, FAST int ni, FAST int nb)
{
    for (FAST int i = 0; i < ni; i++)
      rngBufPut(rid,(char *)buffer,nb);

    for (i = 0; i < ni; i++)
      rngBufGet(rid,(char *)buffer,nb);
}

static NcarTest(FAST Ring &ring, FAST int ni)
{
    for (FAST int i = 0; i < ni; i++)
      ring.Put(buffer);

    for (i = 0; i < ni; i++)
      ring.Get(buffer);
}
