/*
 *	$Id$
 *
 *	Module: mini_vme.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/02/09  18:29:36  craig
 * Initial revision
 *
 *
 * description:
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_MIN_SCOPE
#define scope extern
#include "minDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "minGbl.h"
#include "minDisp.h"
#include "string.h"
#include "taskLib.h"

/************************* MAIN PROGRAM *********************/

void mini_menu()
{
int hxntry;


/* initialize display screen */

init_disp();

/* spawn minirims read task */

/* go(); */
nocbreak();
cmd = 0;
while(cmd != 12)
    {
        scanw("%d",&cmd);
/*      taskDelay(30); */
        switch(cmd)
            {
              case 1:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
/*                  fflush(stdout); */
                  refresh();
                  strcpy(dp->comm_df,"STANDBY");
                  mvaddstr(22,68,dp->comm_df);
/*                  fflush(stdout); */
                  refresh();
/*                *command &= STBY_MASK; */
                  Cmd = STBY;
                  *Command = Cmd;
                  break;
              case 2:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
/*                  fflush(stdout); */
                  refresh();
                  strcpy(dp->comm_df,"RUN");
                  mvaddstr(22,68,dp->comm_df);
/*                  fflush(stdout); */
                  refresh();
                  Cmd = (RUN_GYROS + CAGE_INER + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;

                  break;
              case 3:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"CAGE 0");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  Cmd = (RUN_GYROS + ROT_GIMB_0 + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;
                  break;
              case 4:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"CAGE 90");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  Cmd = (RUN_GYROS + ROT_GIMB_90 + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;
                  break;
              case 5:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"CAGE 180");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  Cmd = (RUN_GYROS + ROT_GIMB_180 + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;
                  break;
              case 6:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"CAGE 270");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  Cmd = (RUN_GYROS + ROT_GIMB_270 + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;
                  break;
              case 7:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"SLEW CCWL");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  scanw("%X",&hxntry);
                  Cmd = hxntry & 0x1ff;
                  *Command = Cmd;
                  refresh();
                  mvcur(0,0,23,68);
                  fflush(stdout);
                  printw("%X",hxntry);
                  refresh();
                  break;
              case 8:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"SLEW CCWH");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  Cmd = (RUN_GYROS + ROT_GIMB_CCW + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;
                  break;
              case 9:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"SLEW CWH");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  Cmd = (RUN_GYROS + ROT_GIMB_CW + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
                  *Command = Cmd;
                  break;
              case 10:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"FREQUENCY");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  break;
              case 11:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"SET TIME");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  break;

              case 12:
                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  strcpy(dp->comm_df,"EXITING");
                  mvaddstr(22,68,dp->comm_df);
                  fflush(stdout);
                  refresh();
                  break;
              default:

                  strcpy(dp->comm_df,"               ");
                  mvaddstr(22,68,dp->comm_df);
                  refresh();
                  strcpy(dp->comm_df,"RE-ENTER");
                  mvaddstr(22,68,dp->comm_df);
                  refresh();
                  break;

            }
        
    }
/* free structure memory */

free(ovp);
free(dp);

scrollok(stdscr, TRUE);
endwin();
exit(0);

}






