/*
 *	$Id$
 *
 *	Module:	gpsFunc.h.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1993/09/21  19:09:44  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:33:47  craig
 * Initial revision
 *
 *
 * description: These are the function definitions used to handle the 
 *              Global Positioning System (GPS) interface.  This interface
 *              consists of a Matrix
 *              CPU-100 Board running software in an EPROM that can handle the  *              Trimble TANS II GPS receiver.
 */

#ifndef INCgpsFunch
#define INCgpsFunch

/***************** Templates of the GPS handling functions ***********/

void init_gps(short);            /* Initalize all of the GPS Pointers */
void command_gps(short);         /* Sends commands to the GPS interface */
void gps_isr(void);              /* GPS mailbox interrupt service routine */
void gps_menu(void);             /* diagnostic menu */

#endif /* INC */



