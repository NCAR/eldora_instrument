/*
 *	$Id$
 *
 *	Module:	paraGbl.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 *
 * Description: This include module contains all of the global 
 *              variables that are used with the parallel input
 *              output board.
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/10/02  17:37:38  craig
 * Initial revision
 *
 * Revision 1.6  1995/09/25  17:45:11  martin
 * updated from Craig's current code
 *
 * Revision 1.3  1995/08/14  14:46:38  craig
 * *** empty log message ***
 *
 * Revision 1.2  1995/06/09  17:33:29  craig
 * *** empty log message ***
 *
 *        
 */

/* Areas to save current setups of each multi-control word */ 
 
scope char curr_pio_d;       /* Current setup of PIO port d */ 
scope char curr_pio_e;       /* Current setup of PIO port e */ 
 
/* Pointer to registers on the parallel I/O card */
scope unsigned char *para_porta;    /* Port A pointer*/
scope unsigned char *para_portb;    /* Port B pointer*/
scope unsigned char *para_portc;    /* Port C pointer*/
scope unsigned char *para_portf;    /* Port F pointer*/
scope char *para_portd;    /* Port D pointer*/
scope char *para_porte;    /* Port E pointer*/
