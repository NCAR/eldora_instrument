/*
 *	$Id$
 *
 *	Module: pio_def.h 		 
 *	Original Author: E. Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/12/09  18:25:15  eric
 * Initial revision
 *
 *
 * description:  Contains global definitions for the Parallel I/O Card.
 *              
 *
 */

#ifndef INCpio_def
#define INCpio_def
#include "ELDRP7.h"

#define PIO_BASE            (0xE000 + SHORT_BASE)
#define IO_PORTA            0x1
#define IO_PORTB            0x3
#define IO_PORTC            0x5
#define IO_PORTD            0x7
#define IO_PORTE            0x9
#define IO_PORTF            0xB
#define IO_CONTROL          0xD


#endif /* INCpio_def */







