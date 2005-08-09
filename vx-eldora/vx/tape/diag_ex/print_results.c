/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/10/25  17:08:08  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:50  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:16  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/************ PRINT DIAGNOSTIC RESULTS BLOCK *****************/

void print_results()
{
printf("RESULTS PAGE CODE= %X\n",rcv_results.page_code);
printf("RESULTS PAGE LENGTH MSB= %X\n",rcv_results.page_length_msb);
printf("RESULTS PAGE LENTGH LSB= %X\n",rcv_results.page_length_lsb);
printf("RESULTS DIAG PARAMS1= %X\n",rcv_results.diag_params1);
printf("RESULTS DIAG PARAMS2= %X\n",rcv_results.diag_params2);
printf("RESULTS DIAG PARAMS3= %X\n",rcv_results.diag_params3);
printf("RESULTS DIAG PARAMS4= %X\n",rcv_results.diag_params4);
printf("RESULTS DIAG PARAMS5= %X\n",rcv_results.diag_params5);
printf("RESULTS DIAG PARAMS6= %X\n",rcv_results.diag_params6);
printf("RESULTS DIAG PARAMS7= %X\n",rcv_results.diag_params7);
printf("RESULTS DIAG PARAMS8= %X\n",rcv_results.diag_params8);
return;
}
