/*
 *	$Id$
 *
 *	Module:		default_colors.h 
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        These are the default colors used for reflectivity and
 * velocity. Note that the encoding is:
 *   red bits 31-24 green 23-16 green 15-8
 *
 */
#ifndef INCdefault_colorsh
#define INCdefault_colorsh

static long defaultColors[] = {
0,				/* Reflectivity colors */
0x3c3c3c00,
0x450000,
0x650a00,
0x9e1e00,
0xb13b00,
0xcd7400,
0xbf9600,
0x9fce00,
0x87fdb00,
0x1c47e800,
0x3830de00,
0x6e0dc600,
0x900cae00,
0xc80f8600,
0xc4438600,
0xc0648700,
0xbf686500,
0xbe6c4400,
0xfac43100,
0xfed92100,
0xfefa0300,
0xfedd1c00,
0xfe9a5800,
0xfe824000,
0xfe5f0500,
0xf94f0800,
0xfd341c00,
0xc8756800,
0xd7b7b500,
0xd2d2d200,
0x2f4f4f00,
0x0,				/* Velocity colors */
0xfe00fe00,
0xfd00fe00,
0xf800fe00,
0xde00fe00,
0xba00fe00,
0xaf00fd00,
0xa500fc00,
0x8b00f800,
0x7101f200,
0x4713ec00,
0x1337e500,
0x6ee500,
0xb6e400,
0x4e89800,
0x2744c00,
0x7d7d7d00,
0xe2c18500,
0xd9953100,
0xeeb81f00,
0xfcda1200,
0xfe6c3a00,
0xfe5d0700,
0xfe560000,
0xfe370000,
0xfe0d0000,
0xfe000000,
0xff000000,
0x2f4f4f00
};

static const int WHITE_LUT = 0xffffffff;
static const int MAX_DEFAULT_COLORS = 66;
static const int MAX_CLUT_SIZE = 256;

#endif INCdefault_colorsh

