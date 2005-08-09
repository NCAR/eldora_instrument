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
 * Revision 1.1  1994/01/06  21:31:35  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:11  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:56  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:20  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>
/*********** INITIALIZE REQUEST SENSE BLOCK **************/

void rqst_sns_init()
{
rqst_sen.vld_err_code=0x00;     
rqst_sen.seg_num=0x00;     
rqst_sen.sense_key=0x00;     
rqst_sen.info_msb=0x00;     
rqst_sen.info_mid_high=0x00;     
rqst_sen.info_mid_low=0x00;     
rqst_sen.info_lsb=0x00;     
rqst_sen.addit_sense_len=0x00;     
rqst_sen.res1=0x00;     
rqst_sen.res2=0x00;     
rqst_sen.res3=0x00;     
rqst_sen.undr_ovr_cntr=0x00;     
rqst_sen.addit_sense_code=0x00;     
rqst_sen.addit_sense_qual=0x00;     
rqst_sen.res5=0x00;     
rqst_sen.res6=0x00;     
rqst_sen.err_ctr_msb=0x00;     
rqst_sen.err_ctr_mid=0x00;     
rqst_sen.err_ctr_lsb=0x00;     
rqst_sen.stat1=0x00;     
rqst_sen.stat2=0x00;     
rqst_sen.stat3=0x00;     
rqst_sen.res7=0x00;     
rqst_sen.rem_tape_msb=0x00;     
rqst_sen.rem_tape_mid=0x00;     
rqst_sen.rem_tape_lsb=0x00;     
rqst_sen.trkng_retry_cntr=0x00;     
rqst_sen.rd_wrt_retry_cntr=0x00;     
rqst_sen.fault_code=0x00;     
}
