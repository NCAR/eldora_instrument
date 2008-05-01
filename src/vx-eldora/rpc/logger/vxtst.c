#define OK_RPC
#define CLIENT_SIDE

#include "vxWorks.h"
#include "logger.h"

void logtst(int c);

void logtst(FAST int c)
{
    FAST int i;
    int j[4];
    FAST char *evm = "Another event!\n";
    FAST char *erm = "It's number %d, followed by %d %d %d!\n";

    if (loggerInit(c) == ERROR)
      {
	  printf("Failed!\n");
	  return;
      }

    j[1] = 1;
    j[2] = 2;
    j[3] = 3;

    for (i = 0; i < 100000; i++)
      {
	  *j = i;
	  loggerEvent(evm,NULL,0);
	  loggerEvent(erm,j,4);
      }
}
