/*
 *
 *	$Id$
 *
 *	Module:	motor_menu.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/09/03  20:04:00  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/09/03  15:56:15  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  20:42:13  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/14  21:34:24  reif
 * Initial revision
 *
 *
 * description:  This module gives the user a menu to test the motor
 *               control functions with from a vx-works prompt. 
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

/* include the .h files that are housekeeper code specific */

#include "tp41vAdr.h"
#include "cntrlDef.h"
#include "cntrlGbl.h"
#include "cntrlFunc.h"


/*************** MAIN PROGRAM ******************/

void motor_menu()
{
unsigned char choice, volt_char;
float position, volts, dd;
float rpm = 0.0;
long i, number;

init_motor();

do
  {
      puts("MOTOR CONTROL TEST PROGRAM");
      puts("PRESS A KEY TO PERFORM THE FOLLOWING FUNCTIONS");
      puts("0) to output 0 volts");
      puts("+) to output a fixed positive voltage");
      puts("-) to output a fixed negative voltage");
      puts("i) to initialize controller");
      puts("p) to read position");
      puts("s) to set velocity");
      puts("g)o motor");
      puts("h)alt motor");
      puts("w)rite to a register");
      puts("r)ead contents of a register");
      puts("f) to clear flags");
      puts("v) to read velocity in rpm");
      puts("G) to set gain");
      puts("C) to set counts");
      puts("P) to set period");
      puts("Q)uit");
      choice=getchar();
      getchar();
      switch(choice)
	{
	  case '0':
	    *mot_prog_cntr=0x01;
	    taskDelay(1);
	    *mot_cmnd=0x80;
	    break;

	  case '+':
	    *mot_prog_cntr=0x01;
	    printf("\nEnter the voltage to output (0 < v < 10): ");
	    scanf("%f",&volts);
	    getchar();
	    if(volts > 10.0) volts = 10.0;
	    if(volts < 0.0) volts = 0.0;
	    volt_char = (unsigned char)(0x80 + (unsigned char)(12.7 * volts));
	    printf("\nRequested value = %f byte written = %x\n",
		   volts,volt_char);
	    *mot_cmnd = volt_char;
	    break;

	  case '-':
	    *mot_prog_cntr=0x01;
	    printf("\nEnter the voltage to output (-10 < v < 0): ");
	    scanf("%f",&volts);
	    getchar();
	    if(volts < -10.0) volts = -10.0;
	    if(volts > 0.0) volts = 0.0;
	    volt_char = (unsigned char)(0x80 + (char)(12.8 * volts));
	    printf("\nRequested value = %f byte written = %x\n",
		   volts,volt_char);
	    *mot_cmnd = volt_char;
	    break;

	  case 'i':
	    init_motor();
	    break;

	  case 'p':
	    printf("\nEnter number of times to read position: ");
	    scanf("%d",&number);
	    getchar();
	    for(i=0; i<number; i++)
	      {
		  position = get_pos();
		  printf("Position that was read = %f\n",position);
		  taskDelay(20);
	      }
	    break;

	  case 's':
	    puts("ENTER VELOCITY DESIRED IN RPM (LESS THAN 30) ");
	    scanf(" %f",&rpm);
	    getchar();
	    if(rpm > 30.0) rpm = 30.;
	    printf("\nThe desired rpm is: %f\n",rpm);
	    set_vel(rpm);
	    break;

	  case 'f':
	    *mot_flag=0x00;
	    taskDelay(1);
	    *mot_flag=0x02;
	    taskDelay(1);
	    *mot_flag=0x03;
	    taskDelay(1);
	    *mot_flag=0x04;
	    taskDelay(1);
	    *mot_flag=0x05;
	    taskDelay(1);
	    printf("FLAG REG= %X\r",*mot_flag&0x0F);
	    break;

	  case 'g':
	    go_motor(); /* GO */
	    break;

	  case 'h':
	    stop_motor(); /* HAULT */
	    break;

	  case 'w':
	    write_reg();
	    break;

	  case 'r':
	    read_reg();
	    break;

	  case 'v':
	    printf("\nEnter number of times to read velocity: ");
	    scanf("%d",&number);
	    getchar();
	    for(i=0; i<number; i++)
	      {
		  rpm = read_rpm();
		  printf("\nThe velocity read was = %f rpm",rpm);
		  taskDelay(20);
	      }
	    break;

          case 'P':
            printf("Enter value for period: ");
            scanf("%f",&dd);
            getchar();
            cntrlPeriod = dd;
            set_vel(rpm);
            break;

          case 'G':
            printf("Enter value for gain: ");
            scanf("%f",&dd);
            getchar();
            cntrlGain = dd;
            set_vel(rpm);
            break;

          case 'C':
            printf("Enter value for counts: ");
            scanf("%f",&dd);
            getchar();
            cntrlCounts = dd;
            set_vel(rpm);
            break;
            
	  case 'q':
	    break;

	  default:
	    puts("NOT A CHOICE! TRY AGAIN");
	    break;
	}
  }while(choice!='q');
}
