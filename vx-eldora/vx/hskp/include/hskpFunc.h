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
 * Revision 1.3  1996/02/21  17:21:14  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/04  19:49:13  craig
 * Initial revision
 *
 * description: These are the function definitions used to handle the 
 *              General Housekeeping processor functions
 */

#ifndef INChskpFunch
#define INChskpFunch

/***************** Templates of the General housekeeper functions ***********/

void hskp(void);               /* main executive program */
void hostStartup(void);        /* starts up all the rest of the tasks */
void startup(void);            /* starts up the rpc tasks */
void start_testpulse(void);    /* Starts the testpulse code */
void update_testpulse(void);   /* Updates the testpulse areas */
void sum_testpulse(void);      /* Sums up testpulse values when called */
void main_menu(void);          /* Main housekeeper menu */

#endif /* INC */







