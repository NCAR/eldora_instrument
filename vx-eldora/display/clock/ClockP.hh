/*
 *	$Id$
 *
 *	Module:		 ClockP.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1991/05/06  15:19:46  thor
 * Changed from 8x16 cells to 12x24 cells. Moved size constants to public
 * view.
 *
 * Revision 1.2  1991/05/03  18:07:23  thor
 * Added comments.
 *
 * Revision 1.1  1991/05/03  15:19:49  thor
 * Initial revision
 *
 *
 *
 * description:
 *        Private data - the default character maps.
 *
 */
#ifndef INCClockPhh
#define INCClockPhh

static unsigned short ZeroMap[] = { 
0x0, 0x0, 0x1f8, 0x3fc,
0x30c, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x30c,
0x3fc, 0x1f8, 0x0, 0x0,
};

static unsigned short OneMap[] = {
0x0, 0x0, 0xe0, 0x1e0,
0x1e0, 0x60, 0x60, 0x60,
0x60, 0x60, 0x60, 0x60,
0x60, 0x60, 0x60, 0x60,
0x60, 0x60, 0x60, 0x60,
0x3fc, 0x3fc, 0x0, 0x0,
};

static unsigned short TwoMap[] = {
0x0, 0x0, 0x3f8, 0x3fc,
0xc, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0xc,
0x18, 0x30, 0x60, 0xc0,
0x180, 0x300, 0x300, 0x200,
0x3fc, 0x3fc, 0x0, 0x0,
};

static unsigned short ThreeMap[] = {
0x0, 0x0, 0x3f8, 0x3fc,
0xc, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0x3f8,
0x3f8, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0xc,
0x3fc, 0x3f8, 0x0, 0x0,
};

static unsigned short FourMap[] = { 
0x0, 0x0, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x3fc,
0x3fc, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0xc,
0xc, 0xc, 0x0, 0x0,
};

static unsigned short FiveMap[] = {
0x0, 0x0, 0x1fc, 0x3fc,
0x300, 0x300, 0x300, 0x300,
0x300, 0x300, 0x300, 0x3f8,
0x3fc, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0xc,
0x3fc, 0x3f8, 0x0, 0x0,
};

static unsigned short SixMap[] = {
0x0, 0x0, 0x1fc, 0x3fc,
0x300, 0x300, 0x300, 0x300,
0x300, 0x300, 0x300, 0x3f8,
0x3fc, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x30c,
0x3fc, 0x1f8, 0x0, 0x0,
};

static unsigned short SevenMap[] = {
0x0, 0x0, 0x3fc, 0x3fc,
0xc, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0x18,
0x18, 0x30, 0x20, 0x60,
0x40, 0xc0, 0x80, 0x180,
0x300, 0x300, 0x0, 0x0,
};

static unsigned short EightMap[] = {
0x0, 0x0, 0x1f8, 0x3fc,
0x30c, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x1f8,
0x1f8, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x30c,
0x3fc, 0x1f8, 0x0, 0x0,
};

static unsigned short NineMap[]= {
0x0, 0x0, 0x1f8, 0x3fc,
0x30c, 0x30c, 0x30c, 0x30c,
0x30c, 0x30c, 0x30c, 0x3fc,
0x1fc, 0xc, 0xc, 0xc,
0xc, 0xc, 0xc, 0xc,
0x3fc, 0x3f8, 0x0, 0x0,
};

static unsigned short ColonMap[] = {
0x0, 0x0, 0x0, 0x0,
0x1f8, 0x1f8, 0x1f8, 0x1f8,
0x1f8, 0x1f8, 0x0, 0x0,
0x0, 0x0, 0x1f8, 0x1f8,
0x1f8, 0x1f8, 0x1f8, 0x1f8,
0x0, 0x0, 0x0, 0x0,
};

#endif 

