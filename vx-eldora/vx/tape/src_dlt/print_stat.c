/*
 *	$Id$
 *
 *	Module: print_stat
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/18  16:03:17  craig
 * Initial revision
 *
 *
 * description: Prints out the status of the requested command
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/******************* PRINT COMMAND STATUS ********************/

void print_stat(int stat,unsigned int cmnd_ident)
{
unsigned short *status_reg;

status_reg = (unsigned short *)(CIP_BASE + SHORT_BASE + CIP_BD_STAT);

if(stat!=0)
 {
     puts("COMMAND TRANSFER NOT COMPLETE!!");
 }
else
  {
      puts("COMMAND TRANSFER COMPLETE");
  }
printf("Parameter block number of command: %2d\n",cmnd_ident);
printf("Ciprico Board Status: 0x%4x\n",*status_reg);
printf("CMD ID = %d\n",parmblk[cmnd_ident]->status_cmd_id);
printf("SCSI STAT = %x\n",parmblk[cmnd_ident]->scsi_status);
printf("ERROR = %x\n",parmblk[cmnd_ident]->error);
printf("STAT FLAGS = %x\n",parmblk[cmnd_ident]->stat_flags);
printf("CLASS CODE = %x\n",parmblk[cmnd_ident]->class_code);
printf("SEGMENT = %x\n",parmblk[cmnd_ident]->segment);
printf("SCSI FLAGS = %x\n",parmblk[cmnd_ident]->scsi_flags);
printf("INFO 3 = %x\n",parmblk[cmnd_ident]->info3);
printf("INFO 4 = %x\n",parmblk[cmnd_ident]->info4);
printf("INFO 5 = %x\n",parmblk[cmnd_ident]->info5);
printf("INFO 6 = %x\n",parmblk[cmnd_ident]->info6);
printf("EX LENGTH = %x\n",parmblk[cmnd_ident]->ex_length);

taskDelay(200);

if(cmnd_ident==MODE_SENSE)
  {
      print_md_sns();
  }

return;
}
