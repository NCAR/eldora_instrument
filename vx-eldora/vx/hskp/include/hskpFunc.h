/*
 *	$Id$
 *
 *	Module:	hskpFunc.h.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/04  19:49:13  craig
 * Initial revision
 *
 * description: These are the function definitions used to handle the 
 *              General Housekeeping processor functions
 */

#ifndef INChskpFunch
#define INChskpFunch

/***************** Templates of the General housekeeper functions ***********/

void hskp(short);              /* main exectutive program */
void start_testpulse();        /* Starts the testpulse code */
void update_testpulse();       /* Updates the testpulse areas */
void main_menu();              /* Main housekeeper menu */

#endif /* INC */






