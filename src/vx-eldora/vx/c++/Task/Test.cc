#include "CDTask.hh"

extern "C" {
#include "stdioLib.h"
#include "usrLib.h"
void taskTest(void);
};

void Test(Task &self, int i);
void Test2(Task &self, int i, int tmo);

static const unsigned int mask = 1;

void taskTest(void)
{
    int arg = 1;

    Task One((FUNCPTR)Test,&arg,1);

    arg++;

    CDTask Two((FUNCPTR)Test,&arg,1);

    arg++;

    Task Three((FUNCPTR)Test,&arg,1,50,3000,VX_FP_TASK | VX_STDIO, 0);

    taskDelay(120);

    One.SetFlags(mask);

    Three.Go();

    Two.SetFlags(mask);

    taskDelay(120);

    Three.SetFlags(mask);

    arg++;

    Task Four((FUNCPTR)Test2,&arg,1);

    printf("Setting priority\n");

    Four.Priority(20);

    printf("Set priority to %d.\n",Four.Priority());

    taskDelay(120);

    Four.SetFlags(mask);

    printf("? ");

    getchar();

    Four.Suspend();

    printf("Task should be suspended.\n");

    taskDelay(120);

    Four.Resume();

    printf("Kill task: ");


    getchar();

    Four.Delete();

    if (Four.Exists() == ERROR)
      printf("We really killed it!\n");
}

void Test(FAST Task &self, FAST int i)
{
    self.FlagsInit();

    printf("I am #%d.\n",i);

    self.WaitOnFlags(mask,FLAGS_AND);

    printf("I still am #%d.\n",i);
}

void Test2(FAST Task &self, FAST int i, FAST int tmo)
{
    self.FlagsInit();

    printf("I am #%d.\n",i);

    self.WaitOnFlags(mask,FLAGS_AND);

    for (;;)
      {
	  ti(taskIdSelf());

	  taskDelay(60);
      }
}

