/*
 *	$Id$
 *
 *	Module:		 builtin.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 17 Jan 1990
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *      This is the header for g++ stdlib stuff that must be reworked
 *   for VxWorks.    
 *
 */
#ifndef INCbuiltinh
#define INCbuiltinh

typedef void (*PF)();

#ifndef BLTIN_LOCAL

IMPORT PF __new_handler;	/* Function pointer for action when */
				/* __builtin_new fails */

#else

void __std_new_handler(void);

PF __new_handler = __std_new_handler;

#endif

/* Functions available to all users */

IMPORT PF set_new_handler(PF);

#endif INCbuiltinh

