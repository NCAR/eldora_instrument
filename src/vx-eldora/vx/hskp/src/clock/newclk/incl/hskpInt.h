/*
 *	$Id$
 *
 *	Module:	hskpints.h	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.7  1999/09/27 16:01:23  eric
 * changed IRQ for GPIB card from 4 to 3.
 *
 * Revision 1.6  1992/11/03  22:53:51  craig
 * *** empty log message ***
 *
 * Revision 1.5  1992/09/04  18:42:54  craig
 * Added levels for all other interfaces
 *
 * Revision 1.4  1992/06/24  20:48:14  shawn
 * periodic safety revision.  Current as of 6/24/92
 *
 * Revision 1.3  1992/06/18  19:55:32  shawn
 * added IRQ levels to each vector.
 *
 * Revision 1.2  1992/06/01  21:54:37  shawn
 * Added vectors 252 and 251, for ecb-spare and ecb DPRAM-contention interrupts
 *
 * Revision 1.1  1992/05/29  17:54:29  shawn
 * Initial revision
 *
 * description: Contains interrupt definitions for the hskp.
 *        
 */

/* VECTOR AND REQUEST LEVEL DEFINITIONS */
#define VME_VME_VEC    255    /* vme-to-vme interface interrupt vector */
#define VME_VME_IRQ      5    /* vme-to-vme interface IRQ level */
#define ECB_CMPLT_VEC  254    /* "ECB command complete" interrupt vector */
#define ECB_CMPLT_IRQ    4    /* "ECB command complete" IRQ level */
#define ECB_ERROR_VEC  253    /* "ECB command error" interrupt vector */
#define ECB_ERROR_IRQ    5    /* "ECB command errot" IRQ level */
#define ECB_SPARE_VEC  252    /* "Spare" ecb interrupt vector */
#define ECB_SPARE_IRQ    4    /* "Spare" ecb IRQ level */
#define ECB_DPCNTD_VEC 251    /* "ECB Master DPRAM contention interrupt */
#define ECB_DPCNTD_IRQ   4    /* "ECB Master DPRAM contention interrupt */
#define IEEE_IRQ         3    /*  IEEE-488 interface IRQ level */
#define ARINC_IRQ        4    /*   ARINC 429 interface IRQ level */
#define ARINC_VEC      249    /*   ARINC 429 interface vector number */
#define GPS_IRQ          4    /*  GPS mailbox interrupt level */
#define MINI_IRQ         6    /*  Interrupt level from clock BIM to
                                   read the MiniRIMS */

