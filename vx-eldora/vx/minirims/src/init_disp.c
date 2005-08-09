#define OK_RPC
#define MIN_SCOPE 
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "minDisp.h"
#include "string.h"
#include "minGbl.h"

/* init_disp initializes the minirim display fields for minirims
 * status and commands.
 *
 * Notes:
 * This version does not use signals.
 * VxWorks version of curses always uses Def_term (there's no getenv()).
 * initscr() seems to require about 41000 bytes from malloc(), so
 *   at least 45000 should be made available.
 * Remember: The OK macro is set by curses.h, so it can't be used for
 *   VxWorks calls anymore! Use VX_OK instead (defined in ../h/stdio.h).
 */


#define TERMINAL "vt100"

#define	NCOLS 80
#define	NLINES 24
#define MAXPATTERNS 4

int init_disp()
{

   /* Initialize curses: set default terminal type (always used on
    * VxWorks) and call initscr(), checking to be sure there's enough
    * malloc() free store. Then set various parameters.
    */
   strcpy(Def_term,TERMINAL);
   if (initscr()==ERR)  {
      fprintf(stderr,"mini_disp: Error from initscr (not enough free store)\n");
      exit(1);
   }
   noecho();
/*   leaveok(stdscr, TRUE); */
   scrollok(stdscr, FALSE);

   /* Initialize Main screen */

   strcpy(ovp->title, "MINI-RIMS  MONITOR  &  CONTROL");
   mvaddstr(TITLE_Y,TITLE_X,ovp->title);
   strcpy(ovp->main_head,"COUNTS                      COMMANDS             PLATFORM STATUS");
   mvaddstr(MAIN_HD_Y,MAIN_HD_X,ovp->main_head);
   strcpy(ovp->acc_gyr_head,"A/F        CHANGE       TOTAL");
   mvaddstr(ACC_GYR_HD_Y,ACC_GYR_HD_X,ovp->acc_gyr_head);
   strcpy(ovp->acc_gyr_sh1,"CHANNELS    (1 sec)      (30 sec)");
   mvaddstr(ACC_GYR_SH1_Y,ACC_GYR_SH1_X,ovp->acc_gyr_sh1);
   strcpy(ovp->acc_gyr_fh1,"X ACCEL");
   mvaddstr(ACC_GYR_FH1_Y,ACC_GYR_FH1_X,ovp->acc_gyr_fh1);
   strcpy(ovp->acc_gyr_fh2,"Y ACCEL");
   mvaddstr(ACC_GYR_FH2_Y,ACC_GYR_FH2_X,ovp->acc_gyr_fh2);
   strcpy(ovp->acc_gyr_fh3,"Z ACCEL");
   mvaddstr(ACC_GYR_FH3_Y,ACC_GYR_FH3_X,ovp->acc_gyr_fh3);
   strcpy(ovp->acc_gyr_fh4,"X GYRO");
   mvaddstr(ACC_GYR_FH4_Y,ACC_GYR_FH4_X,ovp->acc_gyr_fh4);
   strcpy(ovp->acc_gyr_fh5,"Y GYRO");
   mvaddstr(ACC_GYR_FH5_Y,ACC_GYR_FH5_X,ovp->acc_gyr_fh5);
   strcpy(ovp->acc_gyr_fh6,"Z GYRO");
   mvaddstr(ACC_GYR_FH6_Y,ACC_GYR_FH6_X,ovp->acc_gyr_fh6);
   strcpy(ovp->resolver_fh,"RESOLVER");
   mvaddstr(RES_Y,RES_X,ovp->resolver_fh);
   strcpy(ovp->comm1,"1) STANDBY");
   mvaddstr(COMM1_Y,COMM1_X,ovp->comm1);
   strcpy(ovp->comm2,"2) RUN");
   mvaddstr(COMM2_Y,COMM2_X,ovp->comm2);
   strcpy(ovp->comm3,"3) CAGE 0");
   mvaddstr(COMM3_Y,COMM3_X,ovp->comm3);
   strcpy(ovp->comm4,"4) CAGE 90");
   mvaddstr(COMM4_Y,COMM4_X,ovp->comm4);
   strcpy(ovp->comm5,"5) CAGE 180");
   mvaddstr(COMM5_Y,COMM5_X,ovp->comm5);
   strcpy(ovp->comm6,"6) CAGE 270");
   mvaddstr(COMM6_Y,COMM6_X,ovp->comm6);
   strcpy(ovp->comm7,"7) SLEW CCWL");
   mvaddstr(COMM7_Y,COMM7_X,ovp->comm7);
   strcpy(ovp->comm8,"8) SLEW CCWH");
   mvaddstr(COMM8_Y,COMM8_X,ovp->comm8);
   strcpy(ovp->comm9,"9) SLEW CWH");
   mvaddstr(COMM9_Y,COMM9_X,ovp->comm9);
   strcpy(ovp->comm10,"10) FREQUENCY");
   mvaddstr(COMM10_Y,COMM10_X,ovp->comm10);
   strcpy(ovp->comm11,"11) SET TIME");
   mvaddstr(COMM11_Y,COMM11_X,ovp->comm11);
   strcpy(ovp->comm12,"12) QUIT");
   mvaddstr(COMM12_Y,COMM12_X,ovp->comm12);
   strcpy(ovp->pltfm_fh1,"GYRO SPIN");
   mvaddstr(PLTFM_FH1_Y,PLTFM_FH1_X,ovp->pltfm_fh1);
   strcpy(ovp->pltfm_fh2,"GYRO LOOPS");
   mvaddstr(PLTFM_FH2_Y,PLTFM_FH2_X,ovp->pltfm_fh2);
   strcpy(ovp->pltfm_fh3,"GIMBAL LOOP");
   mvaddstr(PLTFM_FH3_Y,PLTFM_FH3_X,ovp->pltfm_fh3);
   strcpy(ovp->pltfm_fh4,"POWER SUPPLY");
   mvaddstr(PLTFM_FH4_Y,PLTFM_FH4_X,ovp->pltfm_fh4);
   strcpy(ovp->temp_head,"TEMPERATURES");
   mvaddstr(TEMP_HD_Y,TEMP_HD_X,ovp->temp_head);
   strcpy(ovp->temp_fh1,"ACCEL A/F");
   mvaddstr(TEMP_FH1_Y,TEMP_FH1_X,ovp->temp_fh1);
   strcpy(ovp->temp_fh2,"X-Y GYRO");
   mvaddstr(TEMP_FH2_Y,TEMP_FH2_X,ovp->temp_fh2);
   strcpy(ovp->temp_fh3,"XR-Z GYRO");
   mvaddstr(TEMP_FH3_Y,TEMP_FH3_X,ovp->temp_fh3);
   strcpy(ovp->temp_fh4,"X ACCEL");
   mvaddstr(TEMP_FH4_Y,TEMP_FH4_X,ovp->temp_fh4);
   strcpy(ovp->temp_fh5,"Y ACCEL");
   mvaddstr(TEMP_FH5_Y,TEMP_FH5_X,ovp->temp_fh5);
   strcpy(ovp->temp_fh6,"Z ACCEL");
   mvaddstr(TEMP_FH6_Y,TEMP_FH6_X,ovp->temp_fh6);
   strcpy(ovp->temp_fh7,"GYRO A/F");
   mvaddstr(TEMP_FH7_Y,TEMP_FH7_X,ovp->temp_fh7);
   strcpy(ovp->temp_fh8,"PLATFORM");
   mvaddstr(TEMP_FH8_Y,TEMP_FH8_X,ovp->temp_fh8);
   strcpy(ovp->time_fh1,"SYSTEM TIME:");
   mvaddstr(TIME_FH1_Y,TIME_FH1_X,ovp->time_fh1);
   strcpy(ovp->time_fh2,"TEST TIME:");
   mvaddstr(TIME_FH2_Y,TIME_FH2_X,ovp->time_fh2);
   strcpy(ovp->time_fh3,"TIME:");
   mvaddstr(TIME_FH3_Y,TIME_FH3_X,ovp->time_fh3);
   strcpy(ovp->time_fh4,"DATE:");
   mvaddstr(TIME_FH4_Y,TIME_FH4_X,ovp->time_fh4);
   strcpy(ovp->comm_fh,"COMMAND:");
   mvaddstr(COMM_FH_Y,COMM_FH_X,ovp->comm_fh);
   fflush(stdout);
   refresh();
}


