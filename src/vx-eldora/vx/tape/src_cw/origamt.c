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
 * Revision 1.3  1992/10/25  17:12:42  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

int start_amnt(char drv_num)
{
unsigned char drive_id;
union
  {
      int start_amt;
      unsigned char starting[4];
  }start;

drive_id = drives_to_use[0];
tst_unt_rdy(drive_id);
exb_cmds(REQUEST_SENSE,RQST_SNS,drv_num);
if(LBOT&rqst_sen.stat1)
	{
	    start.starting[0]=0x00;
	    start.starting[1]=rqst_sen.rem_tape_msb;
	    start.starting[2]=rqst_sen.rem_tape_mid;
	    start.starting[3]=rqst_sen.rem_tape_lsb;
	    
	}
return(start.start_amt);
}
