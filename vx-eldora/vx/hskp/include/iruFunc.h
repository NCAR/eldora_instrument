/*
 *	$Id$
 *
 *	Module:	iruFunc.h.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/03  17:25:30  craig
 * Initial revision
 *
 *
 * description: These are the function definitions used to handle the 
 *              Inertial Reference Unit (IRU) interface.  This interface
 *              consists of an ARINC-429 serial bus interface designed by
 *              NCAR.  This code is specific to the Honeywell LaserRef SM IRU
 */

#ifndef INCiruFunch
#define INCiruFunch

/***************** Templates of the IRU handling functions ***********/

void init_iru(char);        /* Initalize all of the IRU Pointers */
void start_iru();           /* Starts the IRU interface */
void stop_iru();            /* Stops the IRU interface */
void iru_isr();             /* IRU interrupt service routine */

#endif /* INC */



