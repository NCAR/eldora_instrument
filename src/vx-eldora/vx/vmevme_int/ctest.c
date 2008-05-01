#include "getInt.h"

void ctest();

void ctest()
{
  short hbox;

  setupVMEIsr();

  while(1)
    {
      msgQReceive(VMEQue,(char *)&hbox,2,WAIT_FOREVER);

      printf("%d\n",hbox);
    }
}
