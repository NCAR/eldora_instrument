/*
 *	$Id$
 *
 *	Module:	paraDef.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 *
 * Description: This include module contains all of the addresses
 *              that are specific to the parallel input/output card.
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.7  1996/01/25  22:40:18  craig
 * Made changes to handle piezo screw control code
 *
 * Revision 1.6  1995/09/25  17:45:09  martin
 * updated from Craig's current code
 *
 * Revision 1.3  1995/08/14  14:46:37  craig
 * *** empty log message ***
 *
 * Revision 1.2  1995/06/09  17:33:28  craig
 * *** empty log message ***
 *
 *        
 */

/* Base address of all the card */

#define  BASE_PARA     0x0300       /* Parallel Input/Output Card */


/* Offsets for the Parallel Input/Output Card */

#define  PIO_IN_A     0x1       /* Parallel I/O Input Port A */
#define  PIO_IN_B     0x3       /* Parallel I/O Input Port B */
#define  PIO_IN_C     0x5       /* Parallel I/O Input Port C */
#define  PIO_OUT_D    0x7       /* Parallel I/O Output Port D */
#define  PIO_OUT_E    0x9       /* Parallel I/O Output Port E */
#define  PIO_IN_F     0xB       /* Parallel I/O Input Port F */
#define  PIO_CONTRL   0xD       /* Parallel I/O Control Register */

/* Masks for individually defined lines */
#define NM_532_RESET      0x80
#define NM_1064_RESET     0x40
#define TTL_OUT_1         0x20
#define SCREW_PULSE 0x08
#define SCREW_DIR   0x10
