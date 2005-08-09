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
 * Revision 1.3  1992/10/25  17:08:10  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:52  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:18  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/******************* PRINT COMMAND STATUS ********************/

void print_stat(int stat,unsigned int cmnd_ident)
{
if(stat!=0)
 {
     puts("COMMAND TRANSFER NOT COMPLETE!!");
 }
else
  {
      puts("COMMAND TRANSFER COMPLETE");
  }
printf("CMD ID = %d\n",parmblk[cmnd_ident].status_cmd_id);
printf("SCSI STAT = %x\n",parmblk[cmnd_ident].scsi_status);
printf("ERROR = %x\n",parmblk[cmnd_ident].error);
printf("STAT FLAGS = %x\n",parmblk[cmnd_ident].stat_flags);
printf("CLASS CODE = %x\n",parmblk[cmnd_ident].class_code);
printf("SEGMENT = %x\n",parmblk[cmnd_ident].segment);
printf("SCSI FLAGS = %x\n",parmblk[cmnd_ident].scsi_flags);
printf("INFO 3 = %x\n",parmblk[cmnd_ident].info3);
printf("INFO 4 = %x\n",parmblk[cmnd_ident].info4);
printf("INFO 5 = %x\n",parmblk[cmnd_ident].info5);
printf("INFO 6 = %x\n",parmblk[cmnd_ident].info6);
printf("EX LENGTH = %x\n",parmblk[cmnd_ident].ex_length);
taskDelay(200);
if(cmnd_ident==MODE_SENSE)
  {
      print_md_sns();
  }
if(stat!=0)
  {
      exb_cmds(REQUEST_SENSE,RQST_SNS,scsi_id[0]);
      print_rqst_sns();
  }
return;
}
