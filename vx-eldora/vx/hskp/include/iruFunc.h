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
 *
 * description: These are the function definitions used to handle the 
 *              Inertial Reference Unit (IRU) interface.  This interface
 *              consists of an ARINC-429 serial bus interface designed by
 *              VMIC.  This code is specific to the Honeywell LaserRef SM IRU
 */

#ifndef INCiruFunch
#define INCiruFunch

/***************** Templates of the IRU handling functions ***********/

void init_iru(void);           /* Initalize all of the IRU Pointers */
void start_iru(void);          /* Starts the IRU interface */
void convert_iru(void);        /* Function to actually read iru
				  data and convert to proper units */
void calc_iru_pointers(void);  /*calculates the pointer to the iru data */
#endif /* INC */







