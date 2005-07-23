extern "C" {
#include "string.h"
};

char *tst(char *c);

char *tst(char *c)
{
    strcat(c,"sam");

    return(c);
}
