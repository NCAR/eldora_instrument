/*
 *	$Id$
 *
 *	Module vmevme_menuh.c 		 
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
int input, old_count, max_ints;

sysIntEnable(VME_VME_IRQ);
init_vmevme();

do{

    printf(" VME to VME diagnostics menu - Housekeeping processor version\n");
    printf("0) End this menu\n");
    printf("1) Print contents of the VME to VME control registers\n");
    printf("2) Count interrupts from the radar processors\n");
    printf("3) Clear an interrupt that has occured\n");
    scanf(" %d",&input);

    switch(input)
      {
	case 1:
	  printf("Fore Local Status Register:  %2x\n",*fore_local_status);
	  printf("Fore Remote Status Register: %2x\n",*fore_remote_status);

	  printf("Aft Local Status Register:  %2x\n",*aft_local_status);
	  printf("Aft Remote Status Register: %2x\n",*aft_remote_status);

	  break;
	case 2:
	  printf("Enter the number of interrupts to count\n");
	  scanf(" %d",&max_ints);
	  
	  fore_int_cnt = 0;
	  aft_int_cnt = 0;
	  old_count=0;
	  int_count=0;
	  do{
	      if(int_count != old_count)
		{
		    printf("Interrupt number: %d detected\n",int_count);
		    old_count = int_count;
		}
	  }while(int_count < max_ints);
	  printf("Number of interrupts from fore: %d\n",fore_int_cnt);
	  printf("Number of interrupts from aft: %d\n",aft_int_cnt);
	  break;
	case 3:
	  clear_int();
	  break;
	default:
	  break;
      }
}while(input > 0);
}
