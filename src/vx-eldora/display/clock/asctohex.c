#include <stdio.h>

#define FAST register

main(FAST int argc, FAST char **argv)
{
    char input[24];

    FAST int j = 0;
    while (gets(input) != NULL)
      {
	  FAST unsigned short i = (unsigned short)strtol(input,(char **)NULL,
							 2);
	  
	  printf("0x%x, ",(i & 0xffff));
	  
	  if (j == 3)
	    {
		printf("\n");
		j = 0;
	    }
	  else
	    j++;
      }
    exit(0);
}
