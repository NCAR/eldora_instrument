#include "GeGlobal.hh"

extern "C" {
#include "taskLib.h"
#include "stdioLib.h"
#include "rpcLib.h"
void fake(void);
void doAlarm(int);
void htst(void);
};

void fake(void)
{
    FAST DispStatus *st = GeStatus;

    FAST int wait = 1;
    FAST int count = 1;


    for (;;)
      {
	  if (!(count % 60))
	    st->status = 0xffffffff;
	  else if (!(count % 90))
	    st->status = IDLE;

	  count++;

	  taskDelay(wait);
      }
}

void doAlarm(int i)
{
    Alarm->Set(i);
    Alarm->Post();
}

void htst(void)
{
   rpcTaskInit();

   startHeader();

   svc_run();
}
