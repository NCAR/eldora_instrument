/*
 *	$Id$
 *
 *	Module:		 cio.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        These routines manipulate the TP41V cio registers. To use
 * them, specify a register number and port. Valid numbers are 0-3 &
 * valid ports are a, b & c.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "tp41Lib.h"

static unsigned char *CIO0 = (unsigned char *)0xc800000;
static unsigned char *CIO1 = (unsigned char *)0xd000000;
static unsigned char *CIO2 = (unsigned char *)0xd800000;
static unsigned char *CIO3 = (unsigned char *)0xe000000;

static const int A_OFF = 8;
static const int B_OFF = 4;
static const int C_OFF = 0;

void rcio(FAST int select, FAST char *c)
{
    FAST unsigned char *cio;
    FAST unsigned char t;

    if (select == 0)
      cio = CIO0;
    else if (select == 1)
      cio = CIO1;
    else if (select == 2)
      cio = CIO2;
    else if (select == 3)
      cio = CIO3;
    else
      return;

    if (*c == 'A' || *c == 'a')
      cio += A_OFF;
    else if (*c == 'B' || *c == 'b')
      cio += B_OFF;
    else if (*c == 'C' || *c == 'c')
      cio += C_OFF;

    t = *cio;

    printf("0x%x = 0x%x.\n",cio,(int)t & 0xff);
}

void wcio(FAST int select, FAST char *c, FAST unsigned char n)
{
    FAST unsigned char *cio;

    if (select == 0)
      cio = CIO0;
    else if (select == 1)
      cio = CIO1;
    else if (select == 2)
      cio = CIO2;
    else if (select == 3)
      cio = CIO3;
    else
      return;

    if (*c == 'A' || *c == 'a')
      cio += A_OFF;
    else if (*c == 'B' || *c == 'b')
      cio += B_OFF;
    else if (*c == 'C' || *c == 'c')
      cio += C_OFF;

    *cio = n;

    rcio(select,c);
}
