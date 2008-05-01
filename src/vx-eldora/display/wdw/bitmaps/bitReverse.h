/*
 *	$Id$
 *
 *	Module:		 bitReverse.h
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
 *        
 */
#ifndef INCbitReverseh
#define INCbitReverseh

unsigned char reverseChar(unsigned char);
unsigned short reverseShort(unsigned short);
unsigned int reverseInt(unsigned int);
void reverseChars(unsigned char *items, int size);
void reverseShorts(unsigned short *items, int size);
void reverseInts(unsigned int *items, int size);

#define bitsPerByte 8

#endif INCbitReverseh

