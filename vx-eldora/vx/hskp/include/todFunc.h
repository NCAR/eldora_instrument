/*
 *	$Id$
 *
 *	Module:	todFunc.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: The purpose of this module is to test the various functions
 *              of the TIME OF DAY board (TOD).  These include setting the
 *              Real Time Clock (RTC), reading time of power failure, writing
 *              and reading the various register on the RTC, setting up the 
 *              TOD for interrupts using the Bus Interrupter Module (BIM), the 
 *              RTC and the Power Monitor (PM) chip.  The software also
 *              allows the TOD to be set to IRIGB and reading time from the
 *              RTC.
 */

#ifndef INCtodFunch
#define INCtodFunch

/**************** FUNCTION TEMPLATES **************************/

void write_data(void);
void read_data(void);
void tod_menu(void);
void view_time(void);
void synch_to_irigb(void);
void start_clock(void);
void set_time(char,char,char,char,char,char);
void get_time(char *,char *,char *,char *,short *,char *,char *,char *);
short jul_day(int,int,int);
void calc_date(long,int *,int *,int *);

#endif /* INC */




