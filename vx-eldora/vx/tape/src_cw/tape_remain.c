/*
 *	$Id$
 *
 *	Module: orig_amount		 
 *	Original Author: Reif Heck
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Returns the amount of tape remaining on the SCSI drive
 *               Number sent to it
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

int tape_remain(int drv_num)
{
unsigned char drive_id;
union
  {
      int start_amt;
      unsigned char starting[4];
  }start;

drive_id = drv_num;
tst_unt_rdy(drive_id);

exb_cmds(REQUEST_SENSE,RQST_SNS,drive_id);
start.starting[0]=0x00;
start.starting[1]=rqst_sen.rem_tape_msb;
start.starting[2]=rqst_sen.rem_tape_mid;
start.starting[3]=rqst_sen.rem_tape_lsb;
	    
return(start.start_amt);
}
