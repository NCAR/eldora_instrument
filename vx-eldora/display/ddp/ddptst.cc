#include "Ddp.hh"
#include "Task.hh"
#define OK_RPC
#include "DataBeam.hh"

extern "C" {
#include "stdioLib.h"
void ddptst(void);
};

static void ddptask(Task &self);
static void intask(Task &self);

static Pipe *thepipe;

void ddptst(void)
{
    Pipe pipe(sizeof(long *),100);

    int args[1];

    Task it((FUNCPTR)intask,args,0);

    taskDelay(10);

    Task dt((FUNCPTR)ddptask,args,0);
}

static void ddptask(Task &self)
{
    Ddp ddp((void *)0x02000000,243,*thepipe);

    for (;;)
      {
	  ddp.Next();
      }
}

static void intask(Task &self)
{
    Pipe pipe(sizeof(long *),100);

    thepipe = &pipe;

    for (;;)
      {
	  DataBeam *ptr;

	  pipe.Read(&ptr);

	  FAST PLATFORM *platform = &ptr->air;

	  printf("Read 0x%x, angle = %g\n",ptr,platform->rotation_angle);
      }
}
