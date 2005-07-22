/*
 *	$Id$
 *
 *	Module vmevme_menur.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/12  15:07:56  craig
 * Initial revision
 *
 *
 * description:  This function provides the user interface in the housekeeping
 *               processor for the vme to vme diagnostic routines
 */

#define scope

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/* Include needed vx-works .h files */

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <vmevmeDiag.h>
#include <vmevmeAdr.h>
#include <vme_hndshk.h>
#include <tp41vAdr.h>
#include <vmevmeDef.h>
#include <vmevmeGbl.h>
#include <hskpInt.h>

void vmevme_menu()
{
int input, max_ints, i;

init_vmevme();

do{

    printf(" VME to VME diagnostics menu - Radar processor version\n");
    printf("0) End this menu\n");
    printf("1) Print contents of the local register\n");
    printf("2) Cause an interrupt in the housekeeping processor\n");
    printf("3) Cause multiple interrupts in the housekeeping processor\n");
    scanf(" %d",&input);

    switch(input)
      {
	case 1:
	  printf("Local Status Register:  %2x\n",*local_status_reg);
	  break;

	case 2:
	  cause_int();
	  break;

	case 3:
	  printf("Enter the number of interrupts to cause\n");
	  scanf(" %d",&max_ints);
	  for(i=0; i< max_ints; i++)
	    {
		cause_int();
		taskDelay(2);
	    }
	  break;

	default:
	  break;

      }
}while(input > 0);
}
