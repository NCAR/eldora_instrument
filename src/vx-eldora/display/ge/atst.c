#include "vxWorks.h"
#include "stdioLib.h"

atst()
{
    char *ptr = (char *)0xf1020249;

    *ptr = 0xff;
}
