#include "Pipe.hh"

extern "C" {
#include "stdioLib.h"
void catcher(void);
void throw(int i);
};

Pipe *pipe;

void catcher(void)
{
    Pipe p(4,10);
    FAST int j = 10;

    pipe = &p;

    for (FAST int i = 0; i < j; i++)
      {
	  int in;

	  p.Read(&in);

	  printf("Loop %d = %d.\n",i,in);
      }
}

void throw(int i)
{
    pipe->Write(&i);
}
