/*
 *	$Id$
 *
 *	Module: iru_menu.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/05/20  20:36:35  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  15:56:57  craig
 * Initial revision
 *
 * description:  Allows the user to individually test the IRU interface.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#define scope extern

#include "hskpAll.h"

void iru_menu()
{
long *long_pntr, i, k, m;
int cmd, tmp_cmd;
short bd_cmnd, timeout;

union{
  char array[4];
  long aint;
}sch;

init_iru();

/* Just sit in the menu until told to quit */

do{
    printf("\nIRU MENU CHOICES:\n\n");
    printf("0) End IRU menu\n");
    printf("1) Start IRU interface\n");
    printf("2) Initialize IRU Interface\n");
    printf("3) Display Lastest IRU Information\n");
    printf("4) Display Recevied Data Buffer\n");
    printf("5) Display Control/Status Information\n");
    printf("6) Send Commands\n");
    scanf("%d",&cmd);
    getchar();
    printf("command entered: %d\n",cmd);

    switch(cmd)
      {
	case 1:	/* Start IRU interface */
	  start_iru();
	  break;

	case 2:	/* Initialize IRU interface */
	  init_iru();
	  break;

	case 3:	/* Display the latest IRU info */

	  printf("seconds =         %d\n",last_iru_data.sec_longitude);
	  printf("msec_longitude =  %d\n",last_iru_data.msec_longitude);
	  printf("status =          %d\n",last_iru_data.status);
	  printf("latitude =        %f\n",last_iru_data.latitude);
	  printf("longitude =       %f\n",last_iru_data.longitude);
	  printf("wind_speed =      %f\n",last_iru_data.wind_speed);
	  printf("wind_direction =  %f\n",last_iru_data.wind_direction);
	  printf("ns_velocity =     %f\n",last_iru_data.ns_velocity);
	  printf("ew_velocity =     %f\n",last_iru_data.ew_velocity);
	  printf("heading =         %f\n",last_iru_data.heading);
	  printf("drift =           %f\n",last_iru_data.drift);
	  printf("altitude =        %f\n",last_iru_data.altitude);
	  printf("inertial_vspeed = %f\n",last_iru_data.inertial_vspeed);
	  printf("pitch =           %f\n",last_iru_data.pitch);
	  printf("roll =            %f\n",last_iru_data.roll);
	  printf("integ_vert_acc =  %f\n",last_iru_data.integ_vert_acc);
	  printf("vert_acc =        %f\n",last_iru_data.vert_acc);
	  printf("track_rate =      %f\n",last_iru_data.track_rate);
	  printf("pitch_rate =      %f\n",last_iru_data.pitch_rate);
	  printf("Depress Enter to continue\n");
	  getchar();
	  break;

	case 4:	/* Display Received data Buffer */

	  rxa_buffer_offset = iru_pntr->rxa0_vmeptr;
	  i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset;

	  for(k=0; k < 52; k++)
	    {
	      long_pntr = (long *)(i + k*20); 
	      printf("\n%5d ",k*20);
	      for(m=0; m<5; m++)
		printf("%08x ",*(long_pntr + m));
	      for(m=0; m<5; m++)
		{
		  sch.aint = *(long_pntr + m);
		  if(0x7E >= sch.array[0] && sch.array[0] >= 0x20)
		    printf("%c",sch.array[0]);
		  else
		    printf(".");
		  if(0x7E >= sch.array[1] && sch.array[1] >= 0x20)
		    printf("%c",sch.array[1]);
		  else
		    printf(".");
		  if(0x7E >= sch.array[2] && sch.array[2] >= 0x20)
		    printf("%c",sch.array[2]);
		  else
		    printf(".");
		  if(0x7E >= sch.array[3] && sch.array[3] >= 0x20)
		    printf("%c",sch.array[3]);
		  else
		    printf(".");
		} /* for m=0 */
	    }     /* for k=0 */

	  printf("Depress Enter to continue\n");
	  getchar();
	  break;

	case 5:	/* Display Control Area */
	  printf("Board Id             = %4x\n",iru_pntr->board_id);
	  printf("Software rev         = %4x\n",iru_pntr->rev_level);
	  printf("Board Response       = %4x\n",iru_pntr->response);
	  printf("Receiver ID          = %2x\n",iru_pntr->rxid);
	  printf("Transmitter ID       = %2x\n",iru_pntr->txid);
	  printf("Slave interrpt cntrl = %2x\n",iru_pntr->resp_irq);
	  printf("Slave interrpt vec   = %2x\n",iru_pntr->resp_vec);
	  printf("Receiver Mode        = %2x\n",iru_pntr->rx_mode);
	  printf("Transmitters Status  = %2x\n",iru_pntr->tx_bfrst);
	  printf("Receiver Enable Stat = %4x\n",iru_pntr->rx_mask);
	  printf("Rxa0 interrupt cntrl = %2x\n",iru_pntr->rxa0_irq);
	  printf("Rxa0 interrupt vec   = %2x\n",iru_pntr->rxa0_vec);
	  printf("Rxa0 buff ofst addr  = %4x\n",iru_pntr->rxa0_vmeptr);
	  printf("Rxa0 buff limit      = %4d\n",iru_pntr->rxa0_limit);
	  printf("Rxa0 buff ready      = %2x\n",iru_pntr->rxa0_ready);
	  printf("sort type            = %2x\n",iru_pntr->sort_type);
	  printf("Rxa0 comparison      = %4x\n",iru_pntr->rxa0_compare);
	  break;

	case 6:	/* Send a command */
	  printf("\nEnter command to send in decimal: ");
	  scanf("%d",&tmp_cmd);
          bd_cmnd = tmp_cmd;
	  timeout = 0;
	  iru_pntr->response = 0;
	  iru_pntr->command = bd_cmnd;
	  do{
	    taskDelay(1);
	    timeout += 1;
	  }while(iru_pntr->response == 0 && timeout<100);
	  if(iru_pntr->response != COMMAND_ACK)
	 printf("IRU board problem on cmnd: %d. Response: %2x Timeout: %3d\n",
	 bd_cmnd,iru_pntr->response,timeout);
	  else
	    printf("\nCommand successfully sent\n");
	  break;

    default:
      break;
  }
}while(cmd != 0);

return;
}
