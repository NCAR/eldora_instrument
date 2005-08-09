#include "sendInt.h"

void stest();

static char buffer[80];

void stest()
{
  unsigned short ans;
  unsigned short now;

  printf("Enter the number [0 - 26] to be used to trigger the interrupt");
  printf(" or >= 27 to freewheel number 1000 times: ");

  gets(buffer);

  ans = (unsigned short)atoi(buffer);

  if (ans >= 27)
    {
      int i = 0;
      int j = 1000;
      int c = 0;

      for (; i < j; i++)
	{
	  triggerVMEInts(c);

	  c++;
	  if (c >= 27)
	    c = 0;
	  taskDelay(1);
	}
    }
  else
    triggerVMEInts(ans);
}
