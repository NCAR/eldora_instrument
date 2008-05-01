/*
 *	$Id$
 *
 *	Module: mini_menu.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 * description:  Allows the user to individually test the Minirims interface.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void mini_menu()
{

int buf_num, cmd, i, number;
struct gps_data copy_space;
char *cp_pntr, *vme_pntr;
cp_pntr = (char *)&copy_space.gps_data_id[0];  /* Pointer to the copy space */
init_mini();

/* Just sit in the menu until told to quit */

do{
    printf("\nMinirims MENU CHOICES:\n\n");
    printf("0) End Minirims menu\n");
    printf("1) Start Minirims interface\n");
    printf("2) Stop Minirims interface\n");
    printf("3) Initialize the Minirims\n");
    printf("4) Display Minirims Information Summary\n");
    printf("5) Clear the Handshake word\n");
    printf("6) Count Minirims interrupts\n");
    scanf("%d",&cmd);
    getchar();
    switch(cmd)
      {
      case 1:	/* Start Minirims interface */
	  command_mini((short)1);
	  break;

      case 2:	/* Stop Minirims interface */
	  command_mini((short)3);
	  break;

      case 3:	/* Initialize the Minirims interface */
	  init_mini();
	  break;

      case 4:	/* Display Minirims Information */
	  printf("Milli Seconds last fix = %d\n",copy_space.msec_fix);
	  printf("Week Number =            %d\n",copy_space.week_number);
	  printf("Seconds of Week =        %ld\n",copy_space.secs_of_week);
	  printf("UTC offset =             %ld\n",copy_space.utc_offset);
	  break;

      case 5:	/* Clear the handshake word */
	  printf("Clearing handshake word\n");
	  *gps_hndshk = (short)0;
	  break;

      case 6:	/* Count GPS Interrupts */
	  printf("Enter number to count\n");
	  scanf("%d",&number);
	  getchar();
	  for(i=0; i<number; i++)
	    {
		do{}while(!in_gps_isr);
		printf("GPS interrupt #%d detected\n",i);
		in_gps_isr = 0;
	    }
	  break;

      default:
	break;
  }
}while(cmd != 0);

return;
}
