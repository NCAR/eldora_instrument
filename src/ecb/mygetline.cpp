//getline.c
//Drew Leung
//June 26, 2007
//Tom's code

#include <stdio.h>
short mygetline(char *buf)
	{
	short idx;
	char c1;
	
	idx = 0;
	
	while(1)
		{
		c1 = getchar();
		if ((c1 == '\n') || (c1 == '\r'))
			{
			buf[idx] = '\0';
			return(idx);
			}
		if((c1== 0x08) || (c1 == 0x7f))
			{
			if(idx == 0)
				{
				continue;
				}
			idx--;
			putchar(0x08);
			putchar(0x20);
			putchar(0x08);
			continue;
			}
		buf[idx] = c1;
		idx++;
		}
	}


