/*
 *	$Id$
 *
 *	Module: clock_menu.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  2003/09/25  22:15:24  kapoor
 * Needed to define OK_RPC
 *
 * Revision 1.2  2003/09/25  17:26:38  kapoor
 * change of variable names
 *
 * Revision 1.1  2003/09/24  20:48:44  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:32:41  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:35  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.6  1998/09/08  17:34:24  thor
 * *** empty log message ***
 *
 * Revision 1.5  1996/01/24  00:02:48  craig
 * Playing with cvs
 *
 * Revision 1.4  1995/10/27  20:48:14  martin
 * changed HB to HBM
 *
 * Revision 1.3  1995/09/25  17:44:17  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/07/11  21:04:55  craig
 * Initial revision
 *
 *
 *
 * description:  This is the debugging/trouble shooting menu for the 
 *               SABL Bancomm clock printed circuit board.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void clock_menu(void)
{
    int nyear, nmonth, nday, nhour, nminute, nsecond;
    int cmd, count, i;
    char string[20];
    count = 20;

    /* Just sit in the menu until told to quit */

    do{
	printf("\n BANCOMM CLOCK BOARD MENU CHOICES:\n\n");
	printf("0) End clock Menu\n");
	printf("1) Initialize the clock pointers\n");
	printf("2) Change The Repeat Count: %d\n",count);
	printf("3) Display the Current Time\n");
	printf("4) Set The Real Time Clock\n");
	printf("5) Use modulated IRIGB As The Time Source\n");
	printf("6) Use DC level shift IRIGB As The Time Source\n");
	printf("7) Use Real Time Clock as The Time Source\n");
	printf("8) Set Year\n");

	/* read and process the command */

	scanf(" %d",&cmd);
	getchar();

	switch(cmd)
	  {
	    case 1:	/* Initialize the clock pointers */
	      init_clock(0);
	      break;

	    case 2:	/* Change the repeat count */
	      printf("Enter the new repeat count: \n");
	      scanf(" %d",&count);
	      break;
	      
	    case 3:	/* Display the Current time */
	      for(i=0; i<count; i++)
		{
		    get_time(&hr,&min,&sec,&msec,&jday,&mon,&day,&yr);
		    printf("The time (HH:MM:SS:MMM) is: %2d:%2d:%2d:%3d",
			   hr,min,sec,msec);
		    printf(" on (YY/MM/DD): %2d/%2d/%2d  DOY = %3d\n",
			   yr,mon,day,jday);
		    taskDelay(60);
		}
	      break;

	    case 4:	/* Set the real time clock */
	      printf("\nEnter the year: ");
	      scanf(" %d",&nyear);
	      printf("\nEnter the month: ");
	      scanf(" %d",&nmonth);
	      printf("\nEnter the day: ");
	      scanf(" %d",&nday);
	      printf("\nEnter the hour: ");
	      scanf(" %d",&nhour);
	      printf("\nEnter the minute: ");
	      scanf(" %d",&nminute);
	      printf("\nEnter the second: ");
	      scanf(" %d",&nsecond);
	      set_time(nhour, nminute, nsecond, nmonth, nday,
		       nyear);
	      printf("\nDepress enter when: %2d:%2d:%2d is reached\n",
		     nhour,nminute,nsecond);
	      getchar();
	      getchar();
       	      break;

	    case 5:	/* Use modulated IRIG-B as the time source */
	      send_packet("A0");
	      send_packet("HBM");
	      printf("\nModulated IRIG-B is the new time source\n");
	      break;


	    case 6:	/* Use DC level shift IRIG-B as the time source */
	      send_packet("A0");
	      send_packet("HBD");
	      printf("\nDC Level shift IRIG-B is the new time source\n");
	      break;

	    case 7:	/* Use the real time clock as the time source */
	      send_packet("A3");
	      printf("\nThe real time clock is new time source\n");
	      break;
	      
	    case 8:     /* Set year */
	      printf("\nEnter the year (YY): ");
	      scanf(" %d",&nyear);
	      yr = nyear;
	      nyear += 2000;
	      printf("\nThe year has been set to: d%\n",nyear);

	      break;
	      
	    default:
	      break;
	  }
    }while(cmd != 0);

    return;
}






