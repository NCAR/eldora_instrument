/*
 *	$Id$
 *
 *	Module: minvme.c		 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/05/20  20:37:17  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:06:41  reif
 * Initial revision
 *
 *
 * description:
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

/************************* MAIN PROGRAM *********************/

void mini_menu()
{
int choice;
char *array;
char x_accl[]="X_AXIS_ACCEL";
char y_accl[]="Y_AXIS_ACCEL";
char z_accl[]="Z_AXIS_ACCEL";
char accl[]="ft/(sec^2)";
char x_gyro[]="X_AXIS_VEL";
char y_gyro[]="Y_AXIS_VEL";
char z_gyro[]="Z_AXIS_VEL";
char xr_gyro[]="XR_AXIS_VEL";
char vel[]="degrees/sec";
char tmp[]="TEMPERATURE";
char deg[]="degrees C";
unsigned short *reset;
unsigned short *rd_tmp;
unsigned short *ltch_cnt;
unsigned short *min_stat;
unsigned short *rdc;
unsigned short *command;
unsigned short *res_stat;
unsigned short *x_accel;
unsigned short *y_accel;
unsigned short *z_accel;
unsigned short *x_vel;
unsigned short *y_vel;
unsigned short *z_vel;
unsigned short *xr_vel;
unsigned int counter;
unsigned int cmnd;
short *read;
char *units;
float scale;

/****************** INITIALIZE POINTERS *********************/

reset=(unsigned short *)(MIN_BASE+COUNT_RESET); 
rd_tmp=(unsigned short *)(MIN_BASE+READ_TEMP);
ltch_cnt=(unsigned short *)(MIN_BASE+LATCH_COUNT);
min_stat=(unsigned short *)(MIN_BASE+MIN_STATUS);
rdc=(unsigned short *)(MIN_BASE+RESOLVER);
command=(unsigned short *)(MIN_BASE+MIN_COMMAND);
res_stat=(unsigned short *)(MIN_BASE+RESOLVER_STAT);
x_accel=(unsigned short *)(MIN_BASE+X_AXIS_ACCEL);
y_accel=(unsigned short *)(MIN_BASE+Y_AXIS_ACCEL);
z_accel=(unsigned short *)(MIN_BASE+Z_AXIS_ACCEL);
x_vel=(unsigned short *)(MIN_BASE+X_AXIS_VEL);
y_vel=(unsigned short *)(MIN_BASE+Y_AXIS_VEL);
z_vel=(unsigned short *)(MIN_BASE+Z_AXIS_VEL);
xr_vel=(unsigned short *)(MIN_BASE+XR_AXIS_VEL);

/***********************************************************/

do
  {
      puts("MINIRIMS CONTROL MENU");
      puts("ENTER A CHOICE BY NUMBER");
      puts("0) QUIT");
      puts("1) RESET ALL COUNTERS");
      puts("2) READ POSITIVE FREQUENCIES");
      puts("3) READ NEGATIVE FREQUENCIES");
      puts("4) READ STATUS");
      puts("5) WRITE COMMANDS");
      puts("6) READ RESOLVER DATA");
      puts("7) READ RESOLVER STATUS");
      scanf(" %d",&choice);
      switch(choice)
	{
	  case 0:
	    break;
	  case 1:
	    mini_init();
	    break;
	  case 2:
	    printf("ENTER THE COUNTER TO READ IN HEX\n");
	    scanf(" %X",&counter);
	    read=(short *)(MIN_BASE+counter);
	    *reset=0x00;
	    *ltch_cnt=0x00;
	    switch(counter)
	      {
		case X_AXIS_ACCEL:
		  scale=X_ACCEL_SCALE;
		  array=x_accl;
		  units=accl;
		  break;
		case Y_AXIS_ACCEL:
		  scale=Y_ACCEL_SCALE;
		  array=y_accl;
		  units=accl;
		  break;
		case Z_AXIS_ACCEL:
		  scale=Z_ACCEL_SCALE;
		  array=z_accl;
		  units=accl;
		  break;
		case X_AXIS_VEL:
		  scale=X_GYRO_SCALE;
		  array=x_gyro;
		  units=vel;
		  break;
		case Y_AXIS_VEL:
		  scale=Y_GYRO_SCALE;
		  array=y_gyro;
		  units=vel;
		  break;
		case Z_AXIS_VEL:
		  scale=Z_GYRO_SCALE;
		  array=z_gyro;
		  units=vel;
		  break;
		case XR_AXIS_VEL:
		  scale=XR_GYRO_SCALE;
		  array=xr_gyro;
		  units=vel;
		  break;
		case READ_TEMP:
		  scale=TEMP_SCALE;
		  array=tmp;
		  units=deg;
		  break;
		default:
		  printf("WRONG!! TRY AGAIN\n");
		  continue;
	      }
	    for(;;)
	      {
		  *ltch_cnt=0x00;
		  *command=0x000;
		  *command=0x100;
		  taskDelay(55);
		  printf("%s = %15.5f %s      FREQUENCY = %10.0f Hz\r",array,((float)*read*scale),units,(float)*read);
		  if(*read==0)
		    break;		  
	      }
	    break;
	  case 3:
	    printf("ENTER THE COUNTER TO READ IN HEX\n");
	    scanf(" %X",&counter);
	    read=(short *)(MIN_BASE+counter);
	    *reset=0x00;
	    *ltch_cnt=0x00;
	    switch(counter)
	      {
		case X_AXIS_ACCEL:
		  scale=X_ACCEL_SCALE;
		  array=x_accl;
		  units=accl;
		  break;
		case Y_AXIS_ACCEL:
		  scale=Y_ACCEL_SCALE;
		  array=y_accl;
		  units=accl;
		  break;
		case Z_AXIS_ACCEL:
		  scale=Z_ACCEL_SCALE;
		  array=z_accl;
		  units=accl;
		  break;
		case X_AXIS_VEL:
		  scale=X_GYRO_SCALE;
		  array=x_gyro;
		  units=vel;
		  break;
		case Y_AXIS_VEL:
		  scale=Y_GYRO_SCALE;
		  array=y_gyro;
		  units=vel;
		  break;
		case Z_AXIS_VEL:
		  scale=Z_GYRO_SCALE;
		  array=z_gyro;
		  units=vel;
		  break;
		case XR_AXIS_VEL:
		  scale=XR_GYRO_SCALE;
		  array=xr_gyro;
		  units=vel;
		  break;
		case READ_TEMP:
		  scale=TEMP_SCALE;
		  array=tmp;
		  units=deg;
		  break;
		default:
		  printf("WRONG!! TRY AGAIN\n");
		  continue;
	      }
	    for(;;)
	      {
		  *ltch_cnt=0x00;
		  *command=0x100;
		  *command=0x000;
		  taskDelay(55);
		  printf("%s = %15.5f %s   FREQUENCY = %10.0f Hz\r",array,((float)*read*scale),units,(float)*read);
		  if(*read==0)
		    break;		  
	      }
	    break;
	  case 4:
	    printf("MINIRIMS STATUS = %X\n",(*min_stat&0x000F));
	    switch(*min_stat&0x000F)
	      {
		case 0x0000:
		  printf("ALL MINIRIMS STATUS IS BAD!!\n");
	      }
	    break;
	  case 5:
	    printf("ENTER THE COMMAND IN HEX\n");
	    scanf(" %X",&cmnd);
	    *command=cmnd;
	    break;
	  case 6:
	    printf("READING RESOLVER DATA\n");
	    for(;;)
	      {
	      printf("%-06d\r",*rdc);
	      taskDelay(50);
	      }
	    break;
	  case 7:
	    printf("READING RESOLVER AND LATCH STATUS\n");
	    printf("%X\n",(*res_stat&0x000F));
	    break;
	  default:
	    printf("NOT A CHOICE! TRY AGAIN\n");
	    break;
	}
  }while(choice != 0);
}






