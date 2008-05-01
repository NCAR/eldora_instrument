#include "Wdog.hh"

extern "C" {
#include "stdioLib.h"
#include "semLib.h"
void wtst(int d);
};

void f1(SEM_ID s);

void wtst(FAST int d)
{
    SEM_ID s = semBCreate(SEM_EMPTY,SEM_Q_FIFO);

    Wdog w1((FUNCPTR)f1,d,(int)s,1);

    printf("Waiting for w1\n");

    semTake(s,WAIT_FOREVER);

    printf("Triggered!\n");

    w1.Start();

    printf("Waiting for w1\n");

    semTake(s,WAIT_FOREVER);

    printf("Triggered!\n");

}

void f1(FAST SEM_ID s)
{
    semGive(s);
}
