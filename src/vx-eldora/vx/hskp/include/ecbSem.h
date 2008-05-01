/*
 *	$Id$
 *
 *	Module:	ecbSem.h	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/06/24  20:46:33  shawn
 * a version for safety's sake.  Current as of 6/24/92
 *
 * Revision 1.1  1992/05/29  17:52:55  shawn
 * Initial revision
 *
 * description: Contains semaphore definitions for the ecb master.
 *        
 */

#ifdef MAINPROG
SEM_ID ecb_cmd_not_pending;
#else
extern SEM_ID ecb_cmd_not_pending;
#endif





