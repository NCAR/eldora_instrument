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
 * Revision 2.0  1992/11/02  20:42:08  thor
 * First offical ELDORA release!
 *
 * Revision 1.1  1992/01/28  15:44:41  thor
 * Initial revision
 *
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

int rcio(FAST int select, FAST char *c)
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
      return(-1);

    if (*c == 'A' || *c == 'a')
      cio += A_OFF;
    else if (*c == 'B' || *c == 'b')
      cio += B_OFF;
    else if (*c == 'C' || *c == 'c')
      cio += C_OFF;

    t = *cio;

    return((int)t & 0xff);
}

STATUS wcio(FAST int select, FAST char *c, FAST unsigned char n)
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
      return(ERROR);

    if (*c == 'A' || *c == 'a')
      cio += A_OFF;
    else if (*c == 'B' || *c == 'b')
      cio += B_OFF;
    else if (*c == 'C' || *c == 'c')
      cio += C_OFF;

    t = *cio;
    *cio = n;

    return(OK);
}

STATUS setTp41VMEPage(FAST int page)
{
    FAST unsigned char r;
    FAST unsigned char *cio = CIO0 + A_OFF;

    if (page < 0 || page > 7)
      return(ERROR);

    page <<= 3;

    page &= 0x3f;

    r = *cio;

    r |= (unsigned char)page;

    *cio = r;

    return(OK);
}

int getTp41VMEPage(void)
{
    FAST unsigned char r;
    FAST unsigned char *cio = CIO0 + A_OFF;

    r = *cio;

    r >>= 3;

    r &= 0x7;

    return((int)r);
}
