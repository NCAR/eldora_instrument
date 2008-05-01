/*
 *	$Id$
 *
 *	Module:	tod_menu.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/05/20  20:36:46  craig
 * *** empty log message ***
 *
 * Revision 1.2  1994/05/20  20:36:46  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:42:46  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to test the various functions
 *              of the TIME OF DAY board (TOD).  These include setting the
 *              Real Time Clock (RTC), writing
 *              and reading the various registers on the RTC, setting up the 
 *              TOD for interrupts using the Bus Interrupter Module (BIM), the 
 *              RTC and the Power Monitor (PM) chip.  The software also
 *              allows the TOD to be set to IRIGB and reading time from the
 *              RTC.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

/**************************************************************/

void tod_menu()
{
char hour, minute, second, aday, month, year, test;
int ihour, iminute, isecond, iaday, imonth, iyear, itest;
short  millisec, julday, dummy;
unsigned char choice;

/* Initialize all of the global pointers */
init_clock((short)244);

do {
    puts("TOD UTILITY PROGRAM");
    puts("PRESS THE KEY OF YOUR CHOICE");
    puts("W)rite data to TOD board");
    puts("R)ead data from TOD board");
    puts("S)ynch TOD to IRIGB");
    puts("M)anually set clock");
    puts("G)et time");
    puts("V)iew time continuously");
    puts("Q)uit");
    choice = getchar();
    getchar();
    switch(choice) {
      case 'w':
	write_data();
	break;
      case 'r':
	read_data();
	break;
      case 's':
	if((test = sync_irig()) == 0)
	  printf("IRIG-B syncing was unsuccessful\n");
	else
	  printf("IRIG-B syncing was successful\n");
	break;
      case 'm':
	printf("\nEnter the hour in 24 hour format: ");
	scanf(" %d",&ihour);
	printf("\nEnter the minutes: ");
	scanf(" %d",&iminute);

	printf("\nEnter the seconds: ");
	scanf(" %d",&isecond);

	printf("\nEnter the day of month: ");
	scanf(" %d",&iaday);

	printf("\nEnter the month: ");
	scanf(" %d",&imonth);

	printf("\nEnter the year: ");
	scanf(" %d",&iyear);

	hour = (char)ihour;
	minute = (char)iminute;
	second = (char)isecond;
	aday = (char)iaday;
	month = (char)imonth;
	year = (char)iyear;

	printf("\nEnter any number then depress enter to start the clock\n");
	printf("At the time of %2d:%2d:%2d  and date %2d/%2d/%2d\n",
	        ihour,iminute,isecond,imonth,iaday,iyear);
	scanf("%d",&dummy);
	getchar();
	set_time(hour,minute,second,month,aday,year);
	start_clock();
	break;
      case 'g':
	get_time(&hour,&minute,&second,&millisec,&julday,&month,&aday,&year);
	printf("\nTHE TIME IS %02d:%02d:%02d:%02d\n",hour,minute,
	       second,millisec);
	printf("THE DATE IS %02d/%02d/%02d\n",month,aday,year);
	printf("JULIAN DAY IS %d\n",julday);
	break;
      case 'v':
	for(;;)
	  view_time();
	      
      case 'q':
	break;
      default:
	puts("Not a choice. Press any key to resume");
	getchar();
	break;
    }
}while(choice != 'q');
return;
}
