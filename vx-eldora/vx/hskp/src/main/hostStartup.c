/*
 *	$Id$
 *
 *	Module:		 hostStartup.c
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 *
 *
 *
 * description:
 *       This routine runs first in the housekeeping processor and
 *           starts up all of the other routines (tasks) that run in it.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void hostStartup(void)
{
    FILE *fp1;
    char fname[40];

     strcpy(fname,"odin:/vxbin/hskp/scripts/realtime.cmds");
    
    /* Open the file and read all global control flags */
    if(!(fp1=fopen(fname,"r")))
      {
	  printf("Could not find file of global control flags\n");
	  fake_angles = 0;
      }
    else
      {
	  fscanf(fp1,"%1d\n",&fake_angles);
      }
    fclose(fp1);
    printf("The value of fake_angles is: %d\n",fake_angles);
    
    /* Add any other hosts */
    hostAdd("loki","128.117.85.7");
    hostAdd("thor","128.117.85.51");
        
    taskSpawn("startup",100,0,5000,(FUNCPTR)startup,0,0,
	      0,0,0,0,0,0,0,0);
    
    taskSpawn("hskp",100,0,5000,(FUNCPTR)hskp,0,0,
	      0,0,0,0,0,0,0,0);
}

