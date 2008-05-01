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
 * Revision 1.1  1994/01/06  21:31:54  craig
 * Initial revision
 *
 * Revision 1.1  1993/09/22  17:03:38  reif
 * Initial revision
 *
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

void scsi_error(cmnd_ident)
{
char stat;
char drv_num;

switch(parmblk[cmnd_ident].stat_flags)
  {
    case 0xC2:
      printf("EXB #%d COMMAND COMPLETE WITH SOFT ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0xC0:
      printf("EXB #%d COMMAND COMPLETE WITH ERROR\n",cmnd_ident);
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
