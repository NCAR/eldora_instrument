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
printf("4) Turn on Waveguide Switch Complete Revolutions\n");
printf("5) Turn on Waveguide Switch Right Side Only\n");
printf("6) Turn on Waveguide Switch Left Side Only\n");
printf("7) Turn off Waveguide Switch\n");

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

    case 4:      /* Waveguide switch complete revolutions */
      wg_sw_flag = 1;
      wg_sw_current_set = WG_SW_FORE;
      wg_sw_big_angle = 358;
      wg_sw_small_angle = 2;
      wg_sw_counter = 0;
      break;

    case 5:      /* Waveguide switch right side only */
      wg_sw_flag = 2;
      wg_sw_current_set = WG_SW_FORE;
      wg_sw_big_angle = 182;
      wg_sw_small_angle = 178;
      wg_sw_counter = 0;
      break;

    case 6:      /* Waveguide switch left side only */
      wg_sw_flag = 3;
      wg_sw_current_set = WG_SW_FORE;
      wg_sw_big_angle = 358;
      wg_sw_small_angle = 2;
      wg_sw_counter = 0;
      break;

    case 7:      /* turn off Waveguide switch */
      wg_sw_flag = 0;
      break;

    default:
      break;
  }

}





