/*
 *	$Id$
 *
 *	Module: gps_menu.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  1994/05/20  20:36:54  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  15:56:57  craig
 * Initial revision
 *
 * description:  Allows the user to individually test the GPS interface.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void gps_menu()
{

int buf_num, cmd, i, number;
struct gps_data copy_space;
char *cp_pntr, *vme_pntr;
union
  {
      short words[2];
      long add;
  }address;

cp_pntr = (char *)&copy_space.gps_data_id[0];  /* Pointer to the copy space */
init_gps((short)0);

/* Just sit in the menu until told to quit */

do{
    printf("\nGPS MENU CHOICES:\n\n");
    printf("0) End GPS menu\n");
    printf("1) Start GPS interface\n");
    printf("2) Stop GPS interface\n");
    printf("3) Turn on interrupts\n");
    printf("4) Initialize the GPS receiver\n");
    printf("5) Display Time Information\n");
    printf("6) Display Satellite Information\n");
    printf("7) Display Software Information\n");
    printf("8) Display Health Information\n");
    printf("9) Display Position Information\n");
    printf("10) Display Header Information\n");
    printf("11) Display Handshake Information\n");
    printf("12) Display Control Status Information\n");
    printf("13) Clear the Handshake word\n");
    printf("14) Count GPS interrupts\n");
    scanf("%d",&cmd);
    getchar();
    if((cmd > 4) && (cmd < 12))
      {
	  printf("From which buffer? (0,1)\n");
	  scanf("%d",&buf_num);
	  getchar();
	  if(buf_num = 0)
	    vme_pntr = (char *)(STANDARD_BASE + GPS_BASE + 0x10020);
	  else
	    vme_pntr = (char *)(STANDARD_BASE + GPS_BASE + 0x10100);
	  for(i=0; i<sizeof(struct gps_data); i++)
	    *cp_pntr++ = *vme_pntr++;
	  cp_pntr = (char *)&copy_space.gps_data_id[0];
      }

    switch(cmd)
      {
	case 1:	/* Start GPS interface */
	  command_gps((short)1);
	  break;

	case 2:	/* Stop GPS interface */
	  command_gps((short)3);
	  break;

	case 3:	/* Turn on interrupts */
	  init_gps((short)1);
	  break;

    case 4:	/* Initialize the GPS receiver */
	  command_gps((short)2);
	  break;

    case 5:	/* Display Time Information */
	  printf("Milli Seconds last fix = %d\n",copy_space.msec_fix);
	  printf("Week Number =            %d\n",copy_space.week_number);
	  printf("Seconds of Week =        %ld\n",copy_space.secs_of_week);
	  printf("UTC offset =             %ld\n",copy_space.utc_offset);
	  break;

    case 6:	/* Display Satellite Information */
	  printf("Mode as Defined By Trimbal = %lx\n",copy_space.gps_mode);
	  printf("Satellite number 1 =         %d\n",copy_space.satellite[0]);
	  printf("Satellite number 2 =         %d\n",copy_space.satellite[1]);
	  printf("Satellite number 3 =         %d\n",copy_space.satellite[2]);
	  printf("Satellite number 4 =         %d\n",copy_space.satellite[3]);
	  printf("PDOP =                       %f\n",copy_space.pdop);
	  printf("HDOP =                       %f\n",copy_space.hdop);
	  printf("VDOP =                       %f\n",copy_space.vdop);
	  printf("TDOP =                       %f\n",copy_space.tdop);
	  break;

    case 7:	/* Display Software Information */
	  printf("Software version = %d\n",copy_space.software_version);
	  printf("Month =            %d\n",copy_space.month);
	  printf("Day =              %d\n",copy_space.day);
	  printf("Year =             %d\n",copy_space.year);
	  break;

    case 8:	/* Display Health Information */
	  printf("Health as defined by Trimbal =     %x\n",copy_space.health);
	  printf("Machine ID as defined by Trimbal = %x\n",copy_space.machine_id);
	  break;

    case 9:	/* Display Position Information */
	  printf("Latitude (deg) =  %f\n",copy_space.latitude);
	  printf("Longitude (deg) = %f\n",copy_space.longitude);
	  printf("altitude (m) =    %f\n",copy_space.altitude);
	  printf("Clock Bias =  %f\n",copy_space.clock_bias);
	  printf("Time of Fix = %f\n",copy_space.time_of_fix);
	  break;

    case 10:	/* Display Header Information */
	  printf("GPS data ID = %c%c%c%c\n",copy_space.gps_data_id[0],
               copy_space.gps_data_id[1],copy_space.gps_data_id[2],copy_space.gps_data_id[3]);
	  printf("Size of data area (bytes) = %d\n",copy_space.gps_data_len);
	  break;

    case 11:	/* Display Handshake Information */
	  printf("CPU-105 GPS status:        %x\n",*gps_status);
	  address.words[0] = *gps_d_loc_h;
	  address.words[1] = *gps_d_loc_l;
	  printf("CPU-105 GPS Data Location: %lx\n",address.add);
	  printf("CPU-105 GPS command:       %x\n",*gps_command);
	  printf("CPU-105 GPS mailbox flag:  %x\n",*gps_mbox);
	  printf("CPU-105 GPS handshake:     %x\n",*gps_hndshk);
	  break;

    case 12:	/* Display Control Status Information */
	  printf("Fore Radar processor status %x\n",currStatus->fore);
	  printf("Aft Radar processor status %x\n",  currStatus->aft);
	  printf("GPS status word %x           \n",currStatus->gps);
	  printf("Interrupt Detected Flag %d   \n",in_gps_isr);
	  break;

    case 13:	/* Clear the handshake word */
	  printf("Clearing handshake word\n");
	  *gps_hndshk = (short)0;
	  break;

    case 14:	/* Count GPS Interrupts */
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