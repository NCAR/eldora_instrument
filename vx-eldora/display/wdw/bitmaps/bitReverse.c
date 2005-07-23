#include "bitReverse.h"

#define FAST register

unsigned char reverseChar(FAST unsigned char item)
{
    FAST unsigned char target;
    FAST int i;
    FAST int j = sizeof(unsigned char) * bitsPerByte;

    for (i = 0; i < j; i++)
      {
	  target <<= 1;

	  if (item & 1)
	    target |= 1;

	  item >>= 1;
      }
    return(target);
}

unsigned short reverseShort(FAST unsigned short item)
{
    FAST unsigned short target;
    FAST int i;
    FAST int j = sizeof(unsigned short) * bitsPerByte;

    for (i = 0; i < j; i++)
      {
	  target <<= 1;

	  if (item & 1)
	    target |= 1;

	  item >>= 1;
      }
    return(target);
}

unsigned int reverseInt(FAST unsigned int item)
{
    FAST unsigned int target;
    FAST int i;
    FAST int j = sizeof(unsigned int) * bitsPerByte;

    for (i = 0; i < j; i++)
      {
	  target <<= 1;

	  if (item & 1)
	    target |= 1;

	  item >>= 1;
      }
    return(target);
}

void reverseChars(FAST unsigned char *items, FAST int size)
{
    FAST int i;

    for (i = 0; i < size; i++, items++)
      *items = reverseChar(*items);
}

void reverseShorts(FAST unsigned short *items, FAST int size)
{
    FAST int i;

    for (i = 0; i < size; i++, items++)
      *items = reverseShort(*items);
}

void reverseInts(FAST unsigned int *items, FAST int size)
{
    FAST int i;

    for (i = 0; i < size; i++, items++)
      *items = reverseInt(*items);
}
