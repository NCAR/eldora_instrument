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
 * Revision 1.3  1996/02/21  17:21:23  craig
 * *** empty log message ***
 *
 * Revision 1.2  1992/09/01  17:25:25  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:39:58  reif
 * Initial revision
 *
 *
 * description: This is the list of functions associated with 
 *              the TIME OF DAY board (TOD).
 */

#ifndef INCtodFunch
#define INCtodFunch

/**************** FUNCTION TEMPLATES **************************/

scope void write_data(void);
scope void init_clock(short);
scope void read_data(void);
scope void tod_menu(void);
scope void view_time(void);
scope char sync_irig(void);
scope void start_clock(void);
scope void set_time(char,char,char,char,char,char);
scope void get_time(char *,char *,char *,short *,short *,char *,char *,
                    char *);

#endif /* INC */




