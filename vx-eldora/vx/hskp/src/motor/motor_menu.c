/*
 *
 *	$Id$
 *
 *	Module:	motor.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:34:24  reif
 * Initial revision
 *
 *
 * description:
 *             
 *             
 *              
 *              
 *             
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cntrlIncl.h>

/*************** MAIN PROGRAM ******************/

void mtr_menu()
{
unsigned char choice;
float position;
float rpm;


do
  {
      puts("MOTOR CONTROL TEST PROGRAM");
      puts("PRESS A KEY TO PERFORM THE FOLLOWING FUNCTIONS");
      puts("0) to output 0 volts");
      puts("+) to output +10 volts");
      puts("-) to output -10 volts");
      puts("i) to initialize controller");
      puts("p) to read position");
      puts("s) to set velocity");
      puts("g)o motor");
      puts("h)ault motor");
      puts("c) write and read register continuously");
      puts("w)rite to a register");
      puts("r)ead contents of a register");
      puts("f) to clear flags");
      puts("v) to read velocity in rpm");
      puts("Q)uit");
      choice=getchar();
      getchar();
      switch(choice)
	{
	  case '0':
	    *prog_cntr=0x01;
	    taskDelay(1);
	    *cmnd=0x80;
	    break;
	  case '+':
	    *prog_cntr=0x01;
	    taskDelay(1);
	    *cmnd=0x0FF;
	    break;
	  case '-':
	    *prog_cntr=0x01;
	    taskDelay(1);
	    *cmnd=0x01;
	    break;
	  case 'i':
	    init_motor();
	    break;
	  case 'p':
	    get_pos();  
	    break;
	  case 's':
	    puts("ENTER VELOCITY DESIRED IN RPM (LESS THAN 60)");
	    scanf(" %f",&rpm);
	    getchar();
	    set_vel(rpm);
	    break;
	  case 'f':
	    *flag=0x00;
	    taskDelay(1);
	    *flag=0x02;
	    taskDelay(1);
	    *flag=0x03;
	    taskDelay(1);
	    *flag=0x04;
	    taskDelay(1);
	    *flag=0x05;
	    taskDelay(1);
	    printf("FLAG REG= %X\r",*flag&0x0F);
	    break;
	  case 'g':
	    go_motor(); /* GO */
	    break;
	  case 'h':
	    stop_motor(); /* HAULT */
	    break;
	  case 'c':
	    write_reg();
	    break;
	  case 'w':
	    write_data();
	    break;
	  case 'r':
	    read_data();
	    break;
	  case 'v':
	    read_rpm();
	    break;
	  case 'q':
	    break;
	  default:
	    puts("NOT A CHOICE! TRY AGAIN");
	    break;
	}
  }while(choice!='q');
}







