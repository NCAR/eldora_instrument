/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1992/10/30  23:54:40  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/25  17:07:58  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:05  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:06  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>

scsi_error(cmnd_ident)
{

switch(parmblk[cmnd_ident].stat_flags)
  {
    case 0xC2:
      printf("EXB #%d COMMAND COMPLETE WITH SOFT ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0xC0:
      printf("EXB #%d COMMAND COMPLETE WITH ERROR\n",cmnd_ident);
      printf("ERROR RETURNED FROM EXABYTE= %X\n",parmblk[cmnd_ident].scsi_status);
      printf("CIPRICO ERROR STATUS= %X\n",parmblk[cmnd_ident].error);
      printf("CIPRICO BD STATUS= %X\n",*status);
      taskDelay(100);
      break;
    case 0xD0:
      printf("EXB #%d COMMAND COMPLETE WITH TRUNCATED DATA DRV%d\n",drv_num,cmnd_ident);
      stat=1;
      break;
    case 0xE0:
      printf("EXB #%d COMMAND COMPLETE BUT REQUIRED RETRIES\n",cmnd_ident);
      stat=1;
      break;
    case 0x50:
      printf("EXB #%d COMMAND NOT COMPLETE DUE TO TRUNCATED DATA\n",cmnd_ident);
      stat=1;
      break;
    case 0x60:
      printf("EXB #%d COMMAND NOT COMPLETE DUE TO RETRIES\n",cmnd_ident);
      stat=1;
      break;
    case 0x44:
      printf("EXB #%d COMMAND NOT COMPLETE DUE TO ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0x80:
      stat=0;
      break;
  }
}
