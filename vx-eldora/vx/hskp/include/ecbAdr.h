/*
 *	$Id$
 *
 *	Module:	ecbAdr.h	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/06/24  20:45:07  shawn
 * you know, one of those safety revisions for later on
 *
 * Revision 1.1  1992/06/18  20:39:16  shawn
 * Initial revision
 *
 * description: Contains ECB SLAVE addresses (on ECB bus)
 *        
 */

/* ECB addresses (on ECB optical bus)  */
#define ECBRFFOR	2 	/* Rcvr/Xciter Forward */
#define ECBRFAFT	1	/* Rcvr/Xciter Aft */
#define ECBIFFOR	3	/* IF processor Forward */
#define ECBIFAFT	4	/* IF processor Aft */
#define ECBATTEN	5	/* Stepped Attenuator Chassis */

#define ECBMSTRADDR	0x7e	/* Address specifying master (for VME comm only) */
