/*
 *	$Id$
 *
 *	Module:	main_menu	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 *
 *
 * description: This module provides executive control of the ELDORA 
 *              Housekeeping processor by reading a header passed
 *              by the Control Processor and following command passed
 *              by the control processor. 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void main_menu()
{
int cmd;

    printf("Main Menu For The Housekeeper Executive Program\n\n");

    printf("1) Start the Executive program\n");
    printf("2) Stop the Executive program\n");
    printf("3) Reload the Executive program\n");
    scanf("%d",&cmd);
    getchar();
    switch(cmd)
      {
	case 1:         /* Start the executive */
	  stop_flag = 0;
	  break;
	case 2:        /* Stop the executive */
	  stop_flag = 1;
	  break;
	case 3:        /* Reload the executive */
	  reload_flag = 1;
	  break;
	default:
	  break;
      }

}





