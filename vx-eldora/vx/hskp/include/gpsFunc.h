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
 *
 * description: These are the function definitions used to handle the 
 *              Global Positioning System (GPS) interface.  This interface
 *              consists of a Matrix
 *              CPU-100 Board running software in an EPROM that can handle the  *              Trimble TANS II GPS receiver.
 */

#ifndef INCgpsFunch
#define INCgpsFunch

/***************** Templates of the GPS handling functions ***********/

void gps_init();                 /* Initalize all of the GPS Pointers */
void gps_cmnd(short cmnd);       /* Send a command to the GPS interface */
void gps_isr();                  /* GPS mailbox interrupt service routine */

#define scope

#endif /* INC */



