/*
 *	$Id$
 *
 *	Module:		 ecbShowLast
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/17  22:53:32  shawn
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowLast()                                                        */
/*		Read ECB "Last Command" structure in Global memory and       */
/*              print the contents.                                          */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */

void ecbShowLast()
{
printf("ecbShowLast: values in ecbLastCmd global status structure:\n");
printf("ecbShowLast: ecbadr ==> 0x%x, comID ==> 0x%x\n",ecbLastCmd.ecbadr,ecbLastCmd.comID);
}
