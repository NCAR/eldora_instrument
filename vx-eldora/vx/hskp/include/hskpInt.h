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
#define VME-VME_IRQ      1    /* vme-to-vme interface IRQ level */
#define ECB_CMPLT_VEC  254    /* "ECB command complete" interrupt vector */
#define ECB_CMPLT_IRQ    1    /* "ECB command complete" IRQ level */
#define ECB_ERROR_VEC  253    /* "ECB command error" interrupt vector */
#define ECB_ERROR_IRQ    2    /* "ECB command errot" IRQ level */
#define ECB_SPARE_VEC  252    /* "Spare" ecb interrupt vector */
#define ECB_SPARE_IRQ    1    /* "Spare" ecb IRQ level */
#define ECB_DPCNTD_VEC 251    /* "ECB Master DPRAM contention interrupt */
#define ECB_DPCNTD_IRQ   1    /* "ECB Master DPRAM contention interrupt */


